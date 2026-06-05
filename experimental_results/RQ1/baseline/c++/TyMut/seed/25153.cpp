

    #include <functional>
    int main(int argc, char *argv[])
    {
      const int val = 28;
      auto ff = [&]() -> const int& { return val; };
      int i = ff();
    }

