
char x,y;
char& f(){ return x; }
char& g(){ return y; }
void h(){ f()^=g(); }
