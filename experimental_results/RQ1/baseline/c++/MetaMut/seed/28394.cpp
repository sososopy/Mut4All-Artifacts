
    template <int>
    //inline
    int f() {
        return 0;
    }
    
    int g() {
        return f<0>() + 1;
    }

