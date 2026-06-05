
class U{ ~U(); friend void foo(U); };
void foo(U = {});
