
template <bool... B>
bool foo () {
    return ((B && true) || ...);
}

int main () {
    foo<true, false, false, true> ();
} 

