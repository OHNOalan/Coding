int main() {
    asm (
        "mov x0, #0x33 \n mov x16, #1   ;\n svc #0x80     \n"
    );
    // printf("%p", exit);
    // exit(0xafaffafa00ff);
}

// svc #0x80
// pc = 0x0000000182436530  libsystem_kernel.dylib`__exit