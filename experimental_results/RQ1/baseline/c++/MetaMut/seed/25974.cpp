
class TemplateArg1
{
};

class TemplateArg2
{
};

template <typename T1>
class Class1
{
  void f1()
  {
    f2();
  }

  inline void f2()
  {
  }
};


template<>
inline void Class1<TemplateArg1>::f2()
{
}

template<>
inline void Class1<TemplateArg2>::f2()
{
}

template class __attribute__ ((visibility("default"))) Class1<TemplateArg1>;



