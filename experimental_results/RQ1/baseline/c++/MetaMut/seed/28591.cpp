
namespace STD {
class string {
public:
  template <typename T>
  string (const T *);
};
}

export module test;
import "99170_a.H";
export class A {
  STD::string str{"ayyy"};
};


