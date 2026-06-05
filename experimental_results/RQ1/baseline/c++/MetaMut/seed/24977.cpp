
struct Thing
{
  template<typename T>
  Thing& operator=(const T&)
  {
    return *this;
  }
};

