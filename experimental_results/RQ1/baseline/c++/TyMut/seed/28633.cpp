

export module speech;
export const char* get_phrase() {
    return "Hello, world!";
}



import speech;
import <iostream>;
int main() {
    std::cout << get_phrase() << '\n';
}

