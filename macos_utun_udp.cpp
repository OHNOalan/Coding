#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <net/if_utun.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/ioctl.h>
#include <sys/kern_control.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#ifndef PF_SYSTEM
#define PF_SYSTEM 32
#endif
#ifndef AF_SYS_CONTROL
#define AF_SYS_CONTROL 2
#endif
#ifndef SYSPROTO_CONTROL
#define SYSPROTO_CONTROL 2
#endif
#ifndef UTUN_CONTROL_NAME
#define UTUN_CONTROL_NAME "com.apple.net.utun_control"
#endif

// 伪首部，用于计算校验和
struct pseudo_header {
    uint32_t source_address;
    uint32_t dest_address;
    uint8_t placeholder;
    uint8_t protocol;
    uint16_t udp_length;
};

// 校验和计算函数
uint16_t calculate_udp_checksum(struct ip *iph, struct udphdr *udph, unsigned char *payload, int payload_len) {
    struct pseudo_header psh;
    psh.source_address = iph->ip_src.s_addr;
    psh.dest_address = iph->ip_dst.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_UDP;
    psh.udp_length = htons(sizeof(struct udphdr) + payload_len);

    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + payload_len;
    std::vector<uint8_t> check_buf(psize);

    memcpy(check_buf.data(), &psh, sizeof(struct pseudo_header));
    memcpy(check_buf.data() + sizeof(struct pseudo_header), udph, sizeof(struct udphdr));
    memcpy(check_buf.data() + sizeof(struct pseudo_header) + sizeof(struct udphdr), payload, payload_len);

    uint32_t sum = 0;
    uint16_t *ptr = reinterpret_cast<uint16_t *>(check_buf.data());
    int nleft = psize;
    while (nleft > 1) {
        sum += *ptr++;
        nleft -= 2;
    }
    if (nleft == 1) {
        sum += *(reinterpret_cast<uint8_t *>(ptr));
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (uint16_t) (~sum);
}

int open_utun(int unit) {
    int fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (fd < 0) return -1;
    struct ctl_info info;
    memset(&info, 0, sizeof(info));
    strncpy(info.ctl_name, UTUN_CONTROL_NAME, sizeof(info.ctl_name));
    if (ioctl(fd, CTLIOCGINFO, &info) < 0) {
        close(fd);
        return -1;
    }
    struct sockaddr_ctl addr;
    memset(&addr, 0, sizeof(addr));
    addr.sc_len = sizeof(addr);
    addr.sc_family = AF_SYSTEM;
    addr.ss_sysaddr = AF_SYS_CONTROL;
    addr.sc_id = info.ctl_id;
    addr.sc_unit = unit + 1;
    if (connect(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

// 核心回包函数：必须传入整个 buffer
void handle_packet(int fd, unsigned char *full_packet, ssize_t len) {
    // 1. 获取头部指针
    struct ip *ip_hdr = (struct ip *) (full_packet + 4);
    int ip_hdr_len = ip_hdr->ip_hl << 2;
    struct udphdr *udp_hdr = (struct udphdr *) (full_packet + 4 + ip_hdr_len);
    unsigned char *payload = full_packet + 4 + ip_hdr_len + sizeof(struct udphdr);
    int payload_len = ntohs(udp_hdr->uh_ulen) - sizeof(struct udphdr);

    // 2. 交换 IP
    struct in_addr tmp_ip = ip_hdr->ip_src;
    ip_hdr->ip_src = ip_hdr->ip_dst;
    ip_hdr->ip_dst = tmp_ip;

    // 3. 交换端口
    uint16_t tmp_port = udp_hdr->uh_sport;
    udp_hdr->uh_sport = udp_hdr->uh_dport;
    udp_hdr->uh_dport = tmp_port;
    // udp_hdr->uh_dport = htons(8888);

    // 4. 重要：计算校验和
    udp_hdr->uh_sum = 0;
    udp_hdr->uh_sum = calculate_udp_checksum(ip_hdr, udp_hdr, payload, payload_len);

    // 5. 写回完整数据包
    write(fd, full_packet, len);
    std::cout << "  [协议栈] 已注入回显包到 utun 设备" << std::endl;
}

int main() {
    int utun_fd = open_utun(10);
    if (utun_fd < 0) {
        perror("utun open");
        return 1;
    }

    std::cout << "--- 用户态协议栈已启动 ---" << std::endl;
    std::cout << "请配置: sudo ifconfig utun10 10.0.0.1 10.0.0.2 up" << std::endl;

    unsigned char buffer[2048];
    while (true) {
        ssize_t nread = read(utun_fd, buffer, sizeof(buffer));
        if (nread < 4 + (int) sizeof(struct ip)) continue;

        struct ip *ip_hdr = (struct ip *) (buffer + 4);
        if (ip_hdr->ip_p == IPPROTO_UDP) {
            int ip_hdr_len = ip_hdr->ip_hl << 2;
            struct udphdr *udp_hdr = (struct udphdr *) (buffer + 4 + ip_hdr_len);

            std::cout << "\n[捕获包] " << inet_ntoa(ip_hdr->ip_src) << ":" << ntohs(udp_hdr->uh_sport) << " -> "
                      << inet_ntoa(ip_hdr->ip_dst) << ":" << ntohs(udp_hdr->uh_dport) << std::endl;

            // 调用修复后的回包逻辑
            handle_packet(utun_fd, buffer, nread);
        }
    }
    return 0;
}
