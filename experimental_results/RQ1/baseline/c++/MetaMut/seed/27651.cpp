struct string {
    const char * m;
    template<int N> string(const char (&str)[N]) { m = "CONST"; }
    template<int N> string(char (&str)[N]) { m = "MUT"; }
};

string foo()
{
    char bar[8];
    return bar;
}

