
#include <experimental/source_location>
#include <iostream>

using namespace std::experimental;

template<const source_location& X>
void localFunc(const char* name)
{
    static int                     count{0};
    volatile ::std::ios_base::Init dummy{};
    std::cout << "post-main-start: " << name << " " << X.file_name() << " " << X.line()
              << " &count: " << (void*)&count << " count: " << (++count) << " "
              << __PRETTY_FUNCTION__ << std::endl;

    struct Local
    {
        static void init()
        {
            volatile ::std::ios_base::Init dummy{};
            std::cout << "pre-main-start init from localFunc: " << X.file_name() << " "
                      << X.line() << " &count: " << (void*)&count
                      << " count: " << (++count) << " " << X.function_name() << " / " << __PRETTY_FUNCTION__
                      << std::endl;
        }
    };

    static void* volatile initp
        __attribute__((__used__, section(".init_array"))){(void*)&Local::init};   //**** not places into .init_array.
}

#define LOCAL_FUNC(NAME)                                      \
    do                                                        \
    {                                                         \
        constexpr static auto s = source_location::current(); \
        localFunc<s>((const char*)&(NAME)[0]);                \
    } while (0)

void okFunc1() { LOCAL_FUNC("X1"); }
void okFunc2() { LOCAL_FUNC("X2"); }

int main()
{
    std::cout << "main() started" << std::endl;
    okFunc1();
    okFunc2();
    return 0;
}


