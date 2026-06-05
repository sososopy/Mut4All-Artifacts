
template<typename T>
struct C
{
    ~C();
};
template<typename T>
C<T>::~C<T>()      //this is accepted by gcc with c++17
{

}
int main()
{
    C<int> c;;
}
