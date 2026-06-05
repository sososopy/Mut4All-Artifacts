
    int main()
    {
        double xx(0.0);

        // Warning
        bool y = static_cast<bool>(xx);

        // No warning
        bool z = static_cast<bool>(0.0);

        return 0;
    }

