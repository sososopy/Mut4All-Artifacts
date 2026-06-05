
#include <memory>

template<typename Func>
void test_lambda(Func f)
{
}

int main()
{
  std::unique_ptr<int> test_uniq = std::make_unique<int>(0);
  test_lambda([test_uniq = test_uniq.get()] () mutable {
      test_lambda([test_uniq] {
          });
    });
  test_lambda([test_uniq = std::move(test_uniq)] {
      test_lambda([test_uniq = std::move(test_uniq)] {});
    });
}
