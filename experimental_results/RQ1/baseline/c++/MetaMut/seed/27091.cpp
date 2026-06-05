
#include <string>
#include <cstdio>

extern thread_local std::string s;

int main()
{
  puts(s.c_str());
}
$ cat 2.cc
#include <string>

