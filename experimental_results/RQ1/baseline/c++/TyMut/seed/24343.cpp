
void operator "" _s(const char *, unsigned long)
{
}

#define QUOTE(s) #s

int main()
{
    QUOTE("hello"_s);
}
