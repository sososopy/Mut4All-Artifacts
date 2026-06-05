struct X {
  X& operator=(const X&) = delete;
  X& operator=(X&& ) = default;
};

void f(bool t) {
  X a, b;
  *(t ? &a : &b) = X();
  (t ? a : b) = X();
}
-----
The line:

  (t ? a : b) = X();

gives:

error: use of deleted function ‘X& X::operator=(const X&)’

The code compiles with clang++ 3.4. 

The sections in the Standard relevant here are: [expr.cond]/4 "If the second and third operands [of the conditional operator] are glvalues of the same value category and have the same type, the result is of that type and value category" (text in brackets inserted for clarity).

This code was posted at stackoverflow.com/q/19341908/341970
---------------------------------------------------------
The quoted paragraph is implemented in gcc-4.9. However, this still fails. The strange thing is that both of these versions work:

  static_cast<X&>(t ? a : b) = X{};
  (t ? a : b) = static_cast<X&&>(X{});

There is special code in cp_build_modify_expr to handle assignment to a conditional, but it contains this line:

        rhs = stabilize_expr (rhs, &preeval);

which apparently loses the prvalue-ness of rhs.

I think we can consider it as a different issue from PR 53000, because operator?: worked ok, it is the later assignment which failed. By the way, this C++03 version:

struct X {
  private:
  X& operator=(const X&);
  X& operator=(X& );
};

void f(bool t) {
  X a, b;
  *(t ? &a : &b) = X();
  (t ? a : b) = X();
}

says:

c.cc: In function 'void f(bool)':
c.cc:3:6: error: 'X& X::operator=(const X&)' is private
   X& operator=(const X&);
      ^
c.cc:9:18: error: within this context
   *(t ? &a : &b) = X();
                  ^
c.cc:4:6: error: 'X& X::operator=(X&)' is private
   X& operator=(X& );
      ^
c.cc:10:15: error: within this context
   (t ? a : b) = X();
               ^
c.cc:4:6: error: 'X& X::operator=(X&)' is private
   X& operator=(X& );
      ^
c.cc:10:15: error: within this context
   (t ? a : b) = X();
               ^

The fact that it tries to use X::operator=(X&) seems like a bug, which could easily be turned into wrong-code.
---------------------------------------------------------
The C++03 wrong-code version:

struct X {
    X& operator=(const X&){}
    X& operator=(X&){__builtin_abort();}
};

int main(int argv,char**) {
  X a, b;
  ((argv > 2) ? a : b) = X();
}

I don't know if stabilize_expr is wrong, or if it is wrong to call it, I'll leave that to someone else.
---------------------------------------------------------

