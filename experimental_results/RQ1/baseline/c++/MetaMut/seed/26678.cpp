
static constexpr const char root[] = "1234";
static constexpr const char* root2 = "1234";

int main()
{
    static_assert(root < root + 1, "This works");
    static_assert(root2 < root2 + 1, "This fails");
}
