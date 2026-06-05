

class C1 {
        static void f1 ();
};
class C2 {
        public : static void f2 () __attribute__ (( __section__ ("")));
};
inline void C2 :: f2 (){}
void C1 :: f1 (){
        C2 :: f2 ();
}

