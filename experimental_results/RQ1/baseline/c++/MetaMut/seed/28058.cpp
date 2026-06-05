
int main()
{
  FSPI pi;
  FSPG* pg = new FSPG();
  new (&pi.m_a) FSPGI(pg);
  pi.m_a.~FSPGI();
  return 0;
}

