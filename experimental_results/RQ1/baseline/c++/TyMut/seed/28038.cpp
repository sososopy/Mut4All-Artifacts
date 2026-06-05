
struct not_movable {
    not_movable() = default;
    not_movable(not_movable const&) = default;
    not_movable(not_movable&&) = delete;
};

not_movable foo() {
    not_movable obj;
    return obj;
}

