
struct S {};
class S_refwrap {
    S& Sref_;
public:
    S_refwrap(S& Sref) : Sref_(Sref) {}
    operator S&() { return Sref_; }
};

S s;
S_refwrap s_rw(s);
S& s_r(s_rw);
