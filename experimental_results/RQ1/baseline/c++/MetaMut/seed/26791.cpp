

int y;

template<class T>
T val = y;

int main() {
    auto&& x = val<int&>;
}

