

struct E {
    int x;
    E(int x_) : x(x_) {}
    ~E() {}
    E(E&&) = delete;
    E(const E& o) : x(o.x) {}
    E& operator=(const E& o) { x = o.x; return *this; }
};

int main() {
    E ex(1);
    throw ex;
}
