
template<typename... Args>
void spurious(Args... args)
{
    (... + args).member;
}

int main()
{
}
