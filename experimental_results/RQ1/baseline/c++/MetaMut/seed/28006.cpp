
void func1(int x) {}
void func2(char z, bool b) {}

static constexpr void* arr[2] = {(void*)&func1, (void*)func2};
static constexpr void* ptr = (void*)&func1;

int main()
{
    return 0;
}
