
struct sha1;

[[deprecated("old")]]
void flush(sha1&);

void f(sha1& s)
{
  flush(s);
}

