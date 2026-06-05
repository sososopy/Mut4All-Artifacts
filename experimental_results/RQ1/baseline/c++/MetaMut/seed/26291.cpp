
struct s {
auto f() -> decltype(this) { return this; } // error: this inaccessible
void g() {
struct t {
decltype(this) g() { return static_cast<s*>(nullptr); } // OK
auto h() -> decltype(this) { return static_cast<s*>(nullptr); } // Same as previous.
};
}
};
