
#include<string_view>
#include<algorithm>
#include<iostream>

class Test
{
public:
    //for some reason, bug don't appear if std::string_view not wrapped in another class
    class Value
    {
    public:
        constexpr Value(const char*str):data(str){}
        constexpr bool operator<(const Value&value)const{return data<value.data;}

        std::string_view data;
    };

    constexpr Test()
    {
        //no bug if std::sort used
        //std::sort(array,array+8);
        std::make_heap(array,array+8);
        std::sort_heap(array,array+8);
    }

    Value array[8]={"1","2","3","4","5","6","7","8"};
};

int main()
{

    static constexpr Test test;

    for(auto&value:test.array)
        std::cout<<value.data;
    return 0;
}
