

  #include <cassert>
  #include <iostream>

  struct X
  {
    bool alive = true;
    ~X() { alive = false; }

    explicit operator bool() const { return true; }
  } ;

  int main()
  {
    for(int i = 0; X x = X(); assert(x.alive))
      if (++i == 3)
        break;
      else
        std::cout << i << std::endl;
  }

