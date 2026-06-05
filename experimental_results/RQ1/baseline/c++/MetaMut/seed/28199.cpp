
  static int x;
  int main()
  {
      int x = 1;
      {
          extern int x;
          return x;
      }
  }

