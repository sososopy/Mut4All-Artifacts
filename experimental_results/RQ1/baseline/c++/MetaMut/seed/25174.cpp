

typedef int uint32_t;
class A
{
  virtual void m_fn1 ();
};
class B : A
{
  void m_fn1 ();
  enum TrackCommands : uint32_t;
};
enum B::TrackCommands : uint32_t
{
  TRACK_EVENT_UNUSED
};
void
B::m_fn1 ()
{
}

