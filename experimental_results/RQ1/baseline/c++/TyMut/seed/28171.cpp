
template <typename T>
class C {
    class TagType {};
public:
    C(int, TagType = makeTag());
private:
    static TagType makeTag();
};

void test() {
    C<int>(1);
}

