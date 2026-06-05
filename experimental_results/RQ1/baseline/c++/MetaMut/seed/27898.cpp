
template <typename T>
void func(T);

class Cls {
    friend void ::func(int);
};
