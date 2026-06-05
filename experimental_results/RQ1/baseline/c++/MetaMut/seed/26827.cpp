
  struct ExplicitTest
  {
      explicit operator bool() const;
  };

  explicit ExplicitTest::operator bool() const
  {
      return true;
  }

