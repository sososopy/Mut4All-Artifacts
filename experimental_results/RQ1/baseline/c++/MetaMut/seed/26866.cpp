

 namespace N
 {
   int x;
 }

 void f(void)
 {
   _ZN1N1xE = 1; // somehow this is an alias to N::x at the eyes of the C++FE
 }

but rejects other cases that have the same mangling (according to "nm") like

 struct N
 {
   static int x;
 };

 void f(void)
 {
   _ZN1N1xE = 1; // rejected
 }

The following case is not allowed either, which suggests that the mangled name is added into some scope after the declarator has been processed.

 namespace N
 {
   int x = _ZN1N1xE; // rejected
 }

Next case is accepted, again suggesting that after the init-declarator 'x = 3' has been consumed, the mangled name is added into some scope.

 namespace N
 {
   int x = 3, y = _ZN1N1xE; // accepted
 }

