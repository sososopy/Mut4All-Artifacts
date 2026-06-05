
    union  __attribute__((transparent_union)) U {
        int* a;
        ~U() {}
    };

