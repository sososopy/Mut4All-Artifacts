enum { A };
void
foo ()
{
  int x = ({ do {} while (0); A; });
}


