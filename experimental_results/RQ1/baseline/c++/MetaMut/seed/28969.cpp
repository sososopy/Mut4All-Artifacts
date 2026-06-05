//$ cat str.cpp
export module str;

export struct str {
    constexpr str() {
        ptr = new char[42];
    }
    constexpr ~str() { delete[] ptr; }

    const char* ptr = nullptr;
};

export constexpr str get_str() { return str{}; }

//$ cat main.cpp
import str;

int main() {
    str a = get_str();
}

