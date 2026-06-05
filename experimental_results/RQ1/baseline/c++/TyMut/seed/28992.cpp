
import<cstddef>;
void push(std::byte) {}
void write(char v) { push(static_cast<std::byte>(v)); }
int main() { write(char{}); }
