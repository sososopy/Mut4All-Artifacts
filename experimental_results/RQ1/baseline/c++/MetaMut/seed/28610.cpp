
//$ cat z1.cc
class A { operator auto*() -> int; };


//$ cat z2.cc
class { operator auto*() -> int; };


