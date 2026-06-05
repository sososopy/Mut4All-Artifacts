
namespace A { 
    inline int F1 (){}
} 
int F2 ( int (* p )()){ 
    return p ();
}
 int main (){ 
    return F2 ( A :: F1 );
}
