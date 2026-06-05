
struct Test {
    void y() { }
    void x() {
        [](Test &t) { (t.*(&Test::y))(); }(*this);
    }
};  
    
int main() {  Test().x(); }
