
namespace
{
  template <typename WF>
  void
  ml ()
  {
    extern WF cr;

    static_assert (sizeof (cr) == 12, "");
  }

  void
  qc ()
  {
    ml<int[3]> ();
  }
}
