
struct string {
    const char* data;
    string operator+(const char*);
};

int LLVMFuzzerInitialize(int*, char***argv) {
    string ExitOnErr(string(*argv[0]) + ": error:");
    return 0;
}


