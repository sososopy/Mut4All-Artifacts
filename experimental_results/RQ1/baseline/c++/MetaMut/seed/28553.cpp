
typedef struct {
    union {
        struct {
            const void* content;
        } put;
    };
} op_t;

op_t f(const char* alias) {
    return op_t{
        .put =
            {
                .content = alias,
            },
    };
}
