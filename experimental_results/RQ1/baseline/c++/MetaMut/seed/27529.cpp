
template <typename, typename = void_t<>>
struct has_set_attr_method {
    static constexpr bool value = false;
};
template <typename T>
struct has_set_attr_method<T, void_t<decltype(std::declval<T>().setAttr(1))>> {
    static constexpr bool value = true;
};

struct Parent
{
    public:
        template<typename T>
            static void create()    {   
                auto obj = T::create();
                if constexpr(has_set_attr_method<T>::value) {
                    cout << "has setAttr" << endl;
                } else {
                    cout << "no setAttr" << endl;
                }
            }
};

struct Child : public Parent {
    public:
        friend class Parent;
        static auto create() {
            return Child();
        }

    private:
        void setAttr(int x) {
        }
};

int main(int argc, char const *argv[]) {
    Parent::create<Child>();
    return 0;
}
