
struct FunctionBridger
{
    template <typename T>
    FunctionBridger(T& func_)
    {
      T t(func_);
    }
};

struct Function : FunctionBridger
{
    template <typename Ty>
    Function( Ty&& func) : FunctionBridger(func) { }

    ~Function() {}
};

int main()
{
    Function func = 1;
}
