
#include <iostream>
#include <cstring>

template<typename T, char const* TAG>
struct STR  {
    static constexpr const char* tag = TAG;
    STR()  {memset(&v, 0, sizeof(v));}
    STR(T value) {operatorAssign(value);}
    template <class U, std::enable_if_t<!std::is_array<T>::value>* = nullptr>
    T& operatorAssign(const T& rhs) {return v; }
    T v;
};

typedef
class Message {
  public:
    enum severity_t {} ;
    friend std::ostream& operator<< (std::ostream& os, const Message& message) {os << message._severity << ": ";return os;}
    severity_t _severity;
    std::string _message;
} Message_t;

extern char const ctag1[] = "tag1"; 

int main() {
    STR<char[10], ctag1> c1;
    c1 = (char*)"abcdefghij";
}
