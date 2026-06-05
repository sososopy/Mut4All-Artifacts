
template<typename T>
struct target_class
{};

template<typename Class, typename Ret, typename... Args>
struct target_class<Ret (Class::*)(Args...)>
{};

template<typename Class, typename Ret, typename... Args>
struct target_class<Ret (Class::*)(Args...) &>
{};

template<typename Class, typename Ret, typename... Args>
struct target_class<Ret (Class::*)(Args...) &&>
{};
