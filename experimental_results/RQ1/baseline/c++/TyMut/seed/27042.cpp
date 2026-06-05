
int main() {
    int x = 0;
    auto l = [&rx = static_cast<const int&>(x)]() {};
}


