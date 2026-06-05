
struct descriptor_owner_movable {
    descriptor_owner_movable(descriptor_owner_movable&& ) noexcept {}

    // Without destructor it compiles correctly
    ~descriptor_owner_movable() {}
};

descriptor_owner_movable&& declval() noexcept;

int main() {
    static_assert(
        noexcept(descriptor_owner_movable(declval()))
    , "But it is nothrow!");
    return 0;
}
