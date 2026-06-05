
int main(int argc, const char **argv)
{
        auto f = [](const struct __lambda0 &self) { self(self); };
        f(f);
        return 0;
}

