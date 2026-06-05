
class FuncWrapper {
public:
  template <typename Func> void callfunc(Func f)
  {
     f();
  }
};

class Object {
  int field;
public:
  void Method();
  Object() { field = 555; }
};

void Object::Method ()
{
  FuncWrapper wrap;
  wrap.callfunc([]()
		{
		  return Object();
		});
}

