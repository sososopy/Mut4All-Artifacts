
class B
{
public:
    typedef void (B::* T_PTRFUNC)(int);
    virtual void elem_func (int);
    static T_PTRFUNC ptr_func = &B::elem_func;
};

