#include <functional>
#include <iostream>

struct ScopedCounter
{
  int *CountPtr;
  ScopedCounter(int &_Count)
    : CountPtr(&_Count)
  {
    std::cerr << "Construct: " << ++(*CountPtr) << "\n";
  }
  ~ScopedCounter()
  {
    std::cerr << "Destruct: " << --(*CountPtr) << "\n";
  }
  ScopedCounter(ScopedCounter const &_Other)
    : CountPtr(_Other.CountPtr)
  {
    std::cerr << "CopyConstruct: " << ++*CountPtr << "\n";
  }
};
    

struct DispatchTypeTag
{
};

void Capture(std::function<void (DispatchTypeTag *)> const &_Function)
{
  //auto Function = _Function; // Uncomment this and it works
  auto fTest
    = [_Function]()
    {
      _Function(nullptr);
    }
  ;
}

int main()
{
  int Count = 0;
  {
    ScopedCounter Counter(Count);
    Capture
      (
        [Counter](DispatchTypeTag *)
        {
        }
      )
    ;
  }
  if (Count != 0)
    std::cerr << "Count incorrect: " << Count << "\n";
  
  return 0;
}

/* Output does not work

Construct: 1
CopyConstruct: 2
CopyConstruct: 3
CopyConstruct: 4
Destruct: 3
Destruct: 2
Destruct: 1
Count incorrect: 1

*/

/* Output does works
 
Construct: 1
CopyConstruct: 2
CopyConstruct: 3
CopyConstruct: 4
CopyConstruct: 5
Destruct: 4
Destruct: 3
Destruct: 2
Destruct: 1
Destruct: 0
 
*/