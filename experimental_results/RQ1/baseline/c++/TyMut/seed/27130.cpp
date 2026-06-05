
template <class GG>
struct no {
  static void
  tg ()
  {
    void
    hk () noexcept (tg);

    hk ();
  }
};

void
os ()
{
  no<int> ().tg ();
}

