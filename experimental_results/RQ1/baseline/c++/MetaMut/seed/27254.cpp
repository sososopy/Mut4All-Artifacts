
namespace pr {
  template<typename WW>
  void
  kp ()
  {
    extern WW hz;
  }

  void
  n5 ()
  {
    kp<int[]> ();
    kp<int[1]> ();
  }
}

