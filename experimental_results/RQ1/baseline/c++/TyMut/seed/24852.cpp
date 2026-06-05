template<typename T>
struct C
{
     template<typename U> void func(U)
     {

     }
};
int main()
{
    C<int> c;  
    c.template func(5); //this is invalid as per current wording gcc and clang accepts but msvc correctly rejects     
    //c.template func<>(5); //valid as per current wording. All compilers accepts this
}
