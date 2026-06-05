
$ cat regexst.cpp.cpp
struct UnicodeSet {
  int *list = stackList;
  int stackList[];
};
struct RegexStaticSets {
  RegexStaticSets() {}
  UnicodeSet fPropSets[2]{};
};

