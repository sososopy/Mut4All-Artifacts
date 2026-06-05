
#include <string>
#include <cstring>

template<std::size_t N>
struct fixed_string {
    static const constexpr std::size_t size__ = N;

    constexpr fixed_string(char const* s) :
        buf("") {
        for (std::size_t i = 0; i <= N; ++i)
            buf[i] = s[i];
    }
    constexpr operator char const*() const {
        return buf;
    }
    template<std::size_t M>
    constexpr bool compare(const fixed_string<M>& other) const {
        return N == M && ::strncmp(buf, other.buf, N) == 0;
    }

    char buf[N + 1];
};

template<std::size_t N>
fixed_string(char const (&)[N]) -> fixed_string<N - 1>;

template<fixed_string TARGET_NAME, typename TYPE>
concept NameMatcher = (TARGET_NAME.compare(TYPE::name__));

////////////////////////////////////////////

template<fixed_string NAME, typename TYPE>
class Member {
public:
    static const constexpr fixed_string name__ { NAME };

public:
    Member() :
        member_ { } {

    }

    template<fixed_string TARGET_NAME>
    const TYPE& get()
        const requires NameMatcher<TARGET_NAME, TYPE> {
        return member_;
    }

protected:
    TYPE member_;
};

////////////////////////////////////////////

template<typename ... MEMBERS>
class Container: public MEMBERS... {
};

////////////////////////////////////////////

int main(int, char*[]) {
    Container<Member<"fred", int>, Member<"bert", float>,
          Member<"alfie", bool>> container;
    return container.get<"fred">();
}

