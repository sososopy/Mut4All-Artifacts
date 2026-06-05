
struct A { int x; };                    // S(x,A) = {{A::x},{A}}               
struct B { float x; };                  // S(x,B) = {{B::x},{B}}               
struct C: public A, public B {};        // S(x,C) = {{invalid}, {A in C, B in C}}       
struct D: public virtual C {};          // S(x,D) = S(x,C)              
struct E: public virtual C { char x; }; // S(x,E) = {{E::x},{E}}              
struct F: public D, public E {};        // S(x,F) = S(x,E)              
                                                                                
                                                                                
int main ()                                                     
{                                                                               
    F f;                                                                        
    f.x = 0; // OK, lookup finds E::x                                                             
}
