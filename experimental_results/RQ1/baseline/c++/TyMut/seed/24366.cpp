
template <class T>
class Bass {
  T elt;
public:
  static void bing () { }
};

class Deriv : Bass<int>
{
  void boing ()
  {
    Bass::bing ();
  }
};

