
class StatusCode
{
public:
  static const int TEST_VALUE = 0x2;
};

typedef bool AssertionResult;

template <typename T1, typename T2>
AssertionResult CmpHelperEQ(const T1& expected,
                            const T2& actual)
{
  if (expected == actual)
    return true;

  return false;
}

int main()
{
  return CmpHelperEQ(2, StatusCode::TEST_VALUE) ? 0 : 1;
}
