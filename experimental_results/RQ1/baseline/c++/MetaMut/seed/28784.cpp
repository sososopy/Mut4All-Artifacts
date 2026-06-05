
    int i;

    auto coro = [&i, pi = &i](int& i1) -> task {
//        co_await std::suspend_always{};
        std::cout << "&i==&i1: " << std::boolalpha << (&i == &i1) << std::endl;
        std::cout << "&i==pi: " << std::boolalpha << (&i == pi) << std::endl;
        std::cout << "&i1==pi: " << std::boolalpha << (&i1 == pi) << std::endl;
        co_return;
    }(i);
