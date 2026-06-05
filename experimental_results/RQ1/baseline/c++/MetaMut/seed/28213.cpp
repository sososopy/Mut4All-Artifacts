
template<class T>
class is_abstract {
    template<class U>
    static char (&check(U(*)[1]))[2];
    template<class U>
    static char check(...);
public:
    static const bool value = sizeof(check<T>(0)) == sizeof(char);
};
struct abstract {
    virtual void function() = 0;
};
static_assert(is_abstract<abstract>::value, "ok");

