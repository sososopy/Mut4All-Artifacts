
#include <stdio.h>


template <typename _Ty>
    struct mis_const                    {  static void ch() { printf("is const:    false\n"); }  };
    
template <typename _Ty>
    struct mis_const<const _Ty>         {  static void ch() { printf("is const:    true\n"); }  };
    
template <typename _Ty>
    struct mis_volatile                 {  static void ch() { printf("is volatile: false\n"); }  };
    
template <typename _Ty>
    struct mis_volatile<volatile _Ty>   {  static void ch() { printf("is volatile: true\n"); }  };
    
template <typename _Ty>
    struct mis_pointer                  {  static void ch() { printf("is pointer:  false\n"); }  };
    
template <typename _Ty>
    struct mis_pointer<_Ty *>           {  static void ch() { printf("is pointer:  true\n"); }  };
    
    
template <typename _T>
    void show(_T t)
    {        
        printf("----------_T------------\n");
        mis_const<_T>::ch();
        mis_volatile<_T>::ch();
        mis_pointer<_T>::ch();

       
        printf("------decltype(t)-------\n");
        mis_const<decltype(t)>::ch();
        mis_volatile<decltype(t)>::ch();
        mis_pointer<decltype(t)>::ch();
    }
        
int main()
{
    const volatile int *g {nullptr};
    
    show(g);

    return 0;
}
