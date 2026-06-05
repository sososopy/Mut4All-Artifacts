

template<typename T> struct F { };

class a
{
  virtual auto f( F< void () > ) -> void;
  virtual auto g( F< auto () -> void > ) -> void;
};

auto main() -> int { }



