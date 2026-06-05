
struct IntLike
{
  operator int() const
  {
    return 0;
  }
};

template<typename T>
void useAsArrayIndex( const IntLike a )
{
  float myArray[1];
  myArray[ a ];
}

int main()
{
}


