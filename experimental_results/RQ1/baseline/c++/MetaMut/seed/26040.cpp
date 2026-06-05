
struct Class {
        void f () { }
        static void f (int) { }
};

int main ()
{
        [] () { Class::f(0); };
}
