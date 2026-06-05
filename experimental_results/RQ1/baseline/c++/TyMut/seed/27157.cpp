

using T_PTR_FUNC = void (*)(int*);

void func1 (int*);
void func2 (float*);

constexpr T_PTR_FUNC arrFuncs[] =
{ func1, reinterpret_cast<T_PTR_FUNC>(func2) };


