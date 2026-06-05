
class weeble {
public:
    weeble () {}
    class wurble {
        int n;
        wurble (int k) : n(k) {}
    public:
        wurble () : n(0) {}
    };
    wurble fred (int k) {return wurble(k);}
};

int main () {
    weeble a;
    weeble::wurble b = a.fred(42);
}

