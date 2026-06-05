

    const int& test()
    {
        int const static i = 0;
        return i;
    }

    int
    main()
    {
        auto square = [&j = test()]()
        {
        };

        return 0;
    }

