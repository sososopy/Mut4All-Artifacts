
class __declspec(dllexport) A {
public:
    int fa() { return m; }
    int ga();
private:
    int m{0};
};

class __declspec(dllexport) B {
public:
    int fb();
    int gb();
private:
    int m{0};
};
inline int B::fb() { return m; }

class C {
public:
    int fc() { return m; }
    __declspec(dllexport) int gc();
private:
    int m{0};
};



int A::ga() { return (fa() + 1); }

int B::gb() { return (fb() + 1); }

int C::gc() { return (fc() + 1); }

