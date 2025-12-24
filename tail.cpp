#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Linux specific headers
#ifdef __linux__
#include <sys/inotify.h>
#endif

using namespace std;

void tail_file(const string &filepath) {
    ifstream file(filepath);

    // 1. Initial Read (Print existing content)
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    // Clear EOF flag so we can read future data
    file.clear();

#ifdef __linux__
    // === OPTIMIZED WAY (inotify) ===

    // 2. Setup Monitor
    int fd = inotify_init();
    if (fd < 0) perror("inotify_init");

    // Watch for "MODIFY" events on this file
    int wd = inotify_add_watch(fd, filepath.c_str(), IN_MODIFY);

    char buffer[1024]; // Buffer for event data
    while (true) {
        // 3. BLOCK here until OS says "File Changed"
        // Consumes 0% CPU while waiting.
        int length = read(fd, buffer, 1024);

        if (length < 0) {
            perror("read");
            break;
        }

        // 4. File changed! Read new data.
        while (getline(file, line)) {
            cout << "[NEW] " << line << endl;
        }
        file.clear(); // Reset EOF again
    }

    inotify_rm_watch(fd, wd);
    close(fd);

#else
    // === FALLBACK (Polling) for macOS/Windows in interview ===
    // If you are on macOS locally, use this logic:
    cout << "Running in Polling Mode (inotify not available)..." << endl;
    while (true) {
        // Try to read
        bool new_data = false;
        while (getline(file, line)) {
            cout << "[NEW] " << line << endl;
            new_data = true;
        }

        if (!new_data) {
            // Exponential backoff or sleep
            usleep(100000); // 100ms
            file.clear();
        }
    }
#endif
}

int main() {
    // Usage: ./tail my_log.txt
    // Run "echo 'hello' >> my_log.txt" in another terminal to test
    tail_file("my_log.txt");
    return 0;
}
