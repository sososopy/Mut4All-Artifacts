See https://godbolt.org/z/3WMd4T48h.

`mod.cpp`:
```C++
export module mod;
export template<int T=0, class U> void f(U) { }
```

`test.cpp`:
```C++
import mod;
int main() { f(0); }
```

Output:
```
test.cpp: In function 'int main()':
test.cpp:2:15: error: no matching function for call to 'f(int)'
    2 | int main() { f(0); }
      |              ~^~~
In module mod, imported at /app/test.cpp:1:
mod.cpp:2:40: note: candidate: 'template<int T, class U> void f@mod(U)'
    2 | export template<int T=0, class U> void f(U) { }
      |                                        ^
mod.cpp:2:40: note:   template argument deduction/substitution failed:
test.cpp:2:15: note:   couldn't deduce template parameter 'T'
    2 | int main() { f(0); }
      |              ~^~~
```
---------------------------------------------------------

