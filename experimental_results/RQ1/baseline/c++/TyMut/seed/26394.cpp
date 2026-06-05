
struct [[nodiscard]] scope_exit {
        scope_exit(int);
};


int main()
{
    scope_exit{0};
}

