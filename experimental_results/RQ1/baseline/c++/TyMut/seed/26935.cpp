

template<typename... T>
int g(T... t)
{
    return 0;
}

template<class... Args>
void f(Args&&... args)
{    
    auto lm = [&](auto&&..._args) { 
        auto f = [&]{g(std::forward<decltype(_args)>(_args)...);};
      //auto f = [&]{g(_args...);}; /* THIS ONE OK */
        return f(); 
    };
    lm(args...);
}

int main()
{
    f(2, 5, 7);
}

