
struct S3 {
    int operator==(const S3&) const = default;
};
static_assert(S3{} == S3{});
