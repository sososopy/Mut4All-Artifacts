
template<typename T, typename... U>
void bar(T t, U... u) {
    t(u...);
}

template<typename T>
void doit(T) {}

int SIZE = 100;

template<typename ...Vs>
void read(Vs & ...out) {
    char buf[SIZE];
    bar([&buf, &out...](auto&... column) { (doit(column), ...); }, out...);
}

int main() {
    int x1;
    read(x1);
}
