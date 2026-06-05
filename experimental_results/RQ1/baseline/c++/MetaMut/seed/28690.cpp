struct D{};
D global;
struct A{
    explicit operator D(){
        return global;
    }
};
int main(){
    A a;
    D&& rf(a);
}

