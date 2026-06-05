struct V {
 virtual void foo(); 
};

struct wV final : V {
    
};

struct oV final : V {
  void foo();
  
};

void call(V& x)
{
  x.foo();
}

void call(wV& x)
{
  x.foo();
}

void call(oV& x)
{
  x.foo();
}

