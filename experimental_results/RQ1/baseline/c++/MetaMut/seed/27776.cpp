
template <class T>
class widget {
public:
    void swap(widget& rhs) noexcept(noexcept(std::swap(t, rhs.t))) {
        std::swap(t, rhs.t);
    }
private:
    T t;
};
int main() {
    widget<int> w1, w2;
    w1.swap(w2);
}
