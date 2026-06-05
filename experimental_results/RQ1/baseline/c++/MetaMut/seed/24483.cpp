

class A {
        static int aval;
public:
        int get_val();
};

class B : public A {
        static int bval;
public:
        int get_val();
        int bf();
};


int B::bval=5;
int B::get_val() {return bval;};
int B::bf() {return 3;};

int funcB() {
    B b;
    return b.get_val();
}

