
int main()
{
    auto foo = []() __attribute__((cold)) -> bool { return false; };
}
