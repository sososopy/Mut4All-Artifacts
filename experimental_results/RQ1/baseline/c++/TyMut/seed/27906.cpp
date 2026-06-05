

struct xxxx_option
{
    const char *option;
    const char *description;
};

struct xxxx_menu_sub
{
    const char *category;
    struct xxxx_option option [];
};

struct xxxx_menu_sub menu [] =
{
    {
        "MENU1",
        {
            { "a", "AAA" },
            { "b", "BBB" }
        }
    },
    {
        "MENU2",
        {
            { "c", "CCC" },
            { "d", "DDD" }
        }
    }
};

int main ()
{
    return 0;
}//Line 45

