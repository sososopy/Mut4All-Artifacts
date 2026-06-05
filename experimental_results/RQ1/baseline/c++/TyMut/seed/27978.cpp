
template<typename>
concept auto_in_lambda = requires() {
    [](){
        auto x = 4;
    }();
};

static_assert(auto_in_lambda<int>);

