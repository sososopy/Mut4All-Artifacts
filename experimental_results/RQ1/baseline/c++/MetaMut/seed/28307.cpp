
struct Plane { unsigned int bytesused; };

// Passes a reference through. Does not change lifetime.
template <typename Inner>
struct Ref {
    const Inner & i_;
    Ref(const Inner & i) : i_(i) {}
    const Inner & inner() { return i_; }
};

struct FrameMetadata {
    Ref<const Plane> planes() const { return p_; }

    Plane p_;
};

void bar(const Plane & meta);
void foo(const FrameMetadata & fm)
{
    const Plane & meta = fm.planes().inner();
    bar(meta);
}
