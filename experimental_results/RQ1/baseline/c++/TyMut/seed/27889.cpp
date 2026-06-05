

template <class F>
int foo(int argc, char** argv, F&& f){
    #pragma GCC diagnostic push  
    #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    int k;
    if (argv[argc-1]) k=argc;
    return f(k);
    #pragma GCC diagnostic pop
}

auto inner(const int& k) { return k*k; }

int main(int argc, char** argv) {
    [[maybe_unused]] auto lambda = [](const auto& k) { return inner(k);};
    //return foo(argc,argv,inner); //This does compile with -O0 and -O1
    return foo(argc,argv,lambda); //This does _not_ compile with -O1, but does with -O0
}
