
#include <forward_list>
#include <utility>
#include <iostream>

template <class T>
struct two_sided_list : std::forward_list<T>
{
  using std::forward_list<T>::forward_list;
  typename std::forward_list<T>::iterator push_back(T&& t)
  {
    end_ = this->insert_after(end_, std::forward<T>(t));
  }
  T& back()
  {
    return *end_;
  }
private:
  typename std::forward_list<T>::iterator end_ = [this](){this->end();}(); // #1
};

int main()
{
  two_sided_list<int> tsl{1,2,3};
  std::cout << "back is " << tsl.back() << std::endl;
  tsl.push_back(4);
  std::cout << "back is " << tsl.back() << std::endl;
}
