// main.s

.global _main
.align 2

_main:
    // syscall: write(int fd, const void *buf, size_t count)
    
    mov x0, #1              // Argument 1: File Descriptor (1 = stdout)
    adr x1, message         // Argument 2: Address of the string
    mov x2, #13             // Argument 3: Length of string
    mov x16, #4             // Syscall number 4 represents 'write' on macOS
    svc #0x80               // Call kernel

    // syscall: exit(int code)
    mov x0, #0x33           // Exit code 33
    mov x16, #1             // Syscall number 1 is 'exit'
    svc #0x80

message:
    .ascii "Hello, World\n"