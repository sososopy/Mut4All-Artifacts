
template <unsigned int ALIGNMENT>
class foo {
    /* ... */
    private:
        alignas(ALIGNMENT) union bar_ { /* ... */ } bar;
};

