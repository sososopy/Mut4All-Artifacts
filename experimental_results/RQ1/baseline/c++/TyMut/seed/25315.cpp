

#include <string>

class Foo
{
public:
    Foo(std::string string) : m_string(std::move(string)) {}

    std::string & GetString() & { return m_string; }
    std::string && GetString() && { return std::move(m_string); }

    auto & GetStringAuto() & { return m_string; }
    auto && GetStringAuto() && { return std::move(m_string); }

private:
    std::string m_string;
};

int main()
{
    Foo foo("blah");
    std::string fooStringA = foo.GetString();     // works
    std::string fooStringB = foo.GetStringAuto(); // call is ambiguous
    return 0;
}
