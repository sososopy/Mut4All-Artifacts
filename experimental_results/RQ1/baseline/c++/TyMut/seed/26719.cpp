
#include <typeinfo>
#include <tuple>

#define CAT1(A, B) A ## B
#define CAT(A, B) CAT1(A, B)

#define TOKEN_TO_STRING(TOK) # TOK
#define STRINGIZE_TOKEN(TOK) TOKEN_TO_STRING(TOK)


template <char... chars>
using _tstring = std::integer_sequence<char, chars...>;

template <typename T, T... chars>
constexpr _tstring<chars...> operator""_tstr() { return { }; }

#define tstring(STR) decltype( CAT( TOKEN_TO_STRING(STR), _tstr) )



#define dispatch_forward_fn(fn_name, prefix) \
template<class Caller, class Me> \
struct Dispatcher<Caller, tstring(go_up), Me>{ \
    template<class ...Args, class ...ArgsRef> \
    inline decltype(auto) operator() (ArgsRef&&... args) {\
        return 0; \
    } \
};

#define dispatch_list_prefix \
template<class Caller, class Fn_name, class Me> \
struct Dispatcher; \
dispatch_forward_fn(go_up, _d)


dispatch_list_prefix

int main(){
    Dispatcher<void, tstring(go_up), void>()();
    return 0;
}


