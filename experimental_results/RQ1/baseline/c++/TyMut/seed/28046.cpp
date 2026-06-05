
template<typename SX, typename ...XE> void
fk (XE..., SW);

void
w9 (void)
{
  fk<int> (0);
}
