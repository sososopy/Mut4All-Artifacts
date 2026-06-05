template <typename T>
class foo{
public:
    foo(double, char){}
    foo(int) requires requires { foo(0.0, 'c'); }{}
};

int main(){
    foo<int> x(3);
    (void)x;
    return 0;
}

