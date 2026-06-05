template<typename T, typename U>
auto convert(U&& t) {
    // fails - see link to compiler explorer:
    return T{std::forward<U>(t)};
    // succeeds:
    // return T(std::forward<U>(t));
}
