
#include <iostream>
#include <vector>
#include <algorithm>

class FeatureLambdaCaptureThis
{
private:
  int addend;

public:
  FeatureLambdaCaptureThis(int value) : addend{value}
  {
  }
  void DemoLambdaCapture_ThisByValue()
  {
    std::vector<int> values{1, 3, 5, 7, 9, 11, 13, 15};
   
    std::for_each(values.begin(), values.end(), [=, *this](auto &value) mutable {
    value += addend;
        addend++;
     });
    std::cout << "\n" ;
    for (auto &value : values)
    {
      std::cout << " " << value;
    }
    std::cout << std::endl;
  }
};


int main(int argc, char *argv[])
{
  std::cout << "__cplusplus:" << __cplusplus << std::endl;
  FeatureLambdaCaptureThis obj{2};
  obj.DemoLambdaCapture_ThisByValue();
}

