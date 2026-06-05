

#ifdef USE_CONSTEXPR
#define DECL_CONSTEXPR constexpr
#else
#define DECL_CONSTEXPR
#endif

template <typename T>
DECL_CONSTEXPR int typeId();

struct A {};

template<typename T>
struct Template
{
    static void registerType(T t)
    {
        t.does_not_compile;
        (void)typeId<A>();
    }
};

template <typename T>
DECL_CONSTEXPR int typeId()
{
    return 0;
}

void avoidWarning()
{
//     (void)typeId<A>();
}

