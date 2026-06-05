

int testfunc(const char *src, int dstsize)
{
    int dstlen = 0;
    for (; '\0' != *src; ++dstlen, ++src)
    {}
    return dstlen;
}

int main()
{
    return 0;
}

