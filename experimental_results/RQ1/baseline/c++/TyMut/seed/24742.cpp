template <typename...Ts> struct index_by;

template <typename T, typename Indices>
concept InitFunc = true;

// with constrained non-type template parameter
// clang crashes
// gcc is still fine
template <typename Indices, InitFunc<Indices> auto init>
struct LazyLitMatrix;

template <
    typename...Indices,
    InitFunc<index_by<Indices>> auto init /* This parameter pack should be expanded */
>
struct LazyLitMatrix<index_by<Indices...>, init> {
};

LazyLitMatrix<
    index_by<int, char>,
    42
> inst;