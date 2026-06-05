

struct S
{
    constexpr S ()
    : m_ptrNext(nullptr)
    , m_wait(true)
    {}

    S* m_ptrNext;
    bool m_wait;
};



S s_Obj;
S s_Tab[2];


