
struct id {
  id (int);
  operator bool ();
};

id
o9 ()
{
  return id (1) ?: id (2);
}

