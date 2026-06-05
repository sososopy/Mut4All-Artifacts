
#include <functional>

int main(){
  static int array[] = {};

  std::function<void()> func;
  func = [](){
    for(auto i : array){
    }
  };
  return 0;
}


