
template <class T>
concept Container = requires(T t) {
    {
        [](T const& t){
            for(auto&& v : t)
                ;
        }(t)
    };
};

int main() {
    static_assert(!Container<int>);
}

