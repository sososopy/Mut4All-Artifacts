
struct Literal {
    constexpr Literal() {}
    
    // This is allowed (not const), but perhaps shouldn't be.
    bool has_value = false;
};

template<Literal lit>
void Test() {
    // The below will fail if uncommented, but seems different
    // from the language of the spec.

    // error: assignment of member 'Literal::has_value' in read-only object
    // lit.has_value = true; 
}

int main()
{
    Test<Literal{}>();
}
