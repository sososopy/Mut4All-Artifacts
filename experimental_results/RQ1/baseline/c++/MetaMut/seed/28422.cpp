
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma GCC push_options
#pragma GCC optimize("O0")
#endif

void gcc_10_1_0_is_dumb(const std::vector<std::string> &files,
        const int N,
        char *types,
        rtosc_arg_t *args)
{
        types[N] = 0;
        for(int i=0; i<N; ++i) {
            args[i].s = files[i].c_str();
            types[i]  = 's';
        }
}

#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma GCC pop_options
#endif

