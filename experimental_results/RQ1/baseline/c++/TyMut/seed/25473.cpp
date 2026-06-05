
struct Error {
~Error();
};

__attribute__((warn_unused_result)) Error test();

int main()
{
    test();
    return 0;
}

