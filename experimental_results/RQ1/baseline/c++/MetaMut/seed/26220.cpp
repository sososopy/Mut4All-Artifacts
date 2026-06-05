
int dummyd()
{
    return 110;
}

int __attribute__((__stdcall__)) dummye()
{
    return 1;
}

template<typename T>
bool Decider(T && )
{
    return true;
}

int main()
{
    Decider(dummyd);
    Decider(dummye);
    return 0;
}
