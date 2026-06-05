template<typename T>
concept X = true;

template <X T>
class Y {
public:
    Y(T y)  {  }

    template <X U>
    friend class Y;
};

template class Y<int>;

int main() {
    Y y(0);
}