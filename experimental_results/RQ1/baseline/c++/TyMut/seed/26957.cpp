
    #include <iostream>
    #include <limits>

    struct S {
      int x;
      int y;
    };

    S const data[] = {
      { 1, std::numeric_limits<int>::max() }
    };

    int main() {
      S data2[] = {
        data[0]
      };

      std::cout
        << data [0].x << ", " << data [0].y << "\n"
        << data2[0].x << ", " << data2[0].y << "\n"
        ;
    }

