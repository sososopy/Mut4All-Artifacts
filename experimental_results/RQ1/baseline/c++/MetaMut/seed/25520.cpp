
    #include <future>
    #include <iostream>

    int main()
    {
        std::async(
            []() {}
        ).get();

        std::cout << "success\n";
    }

