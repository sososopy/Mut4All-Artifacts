
my_coro ()
{
  const std::array<int, 5> expectedValues = {{0, 3, 1, 4, 2}};

  for (int expectedValue : expectedValues) {
    co_yield expectedValue;
  }
}
