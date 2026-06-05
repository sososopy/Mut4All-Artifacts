
struct eo {
  eo () { a6 = new int; }
  ~eo () { delete a6; }
  void cl () { bf = 0; }

  int *a6;
  int bf;
};

struct ja {
  ja (int *k9) : ia (k9) { }
  ja (ja &);
  bool operator!= (ja w4) { return w4.ia; }

  int *ia;
};

struct vi {
  vi () { sb = new eo; }
  ~vi () { delete sb; }
  ja x6 () { return sb->a6; }

  vi
  da ()
  {
    vi ns;
    ja qf = nullptr;

    while (qf != x6 ())
      ns.sb->cl ();

    return ns;
  }

  eo *sb;
};

vi
fi ()
{
  return vi ().da ();
}

