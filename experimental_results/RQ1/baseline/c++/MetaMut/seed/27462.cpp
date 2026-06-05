
 enum LockHistory : bool { Yes, No };

 constexpr bool to_bool(LockHistory e)
 {
     return e;
 }

 int main()
 {
     const LockHistory x = static_cast<LockHistory>(2);
     static_assert(to_bool(x), "fail"); // The static assert fails in GCC, but OK in clang
 }

