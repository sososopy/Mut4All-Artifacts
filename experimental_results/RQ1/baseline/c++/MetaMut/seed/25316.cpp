
template <typename _Tp> struct A { typedef __underlying_type(_Tp) type; };
namespace o3tl {
template <typename> struct typed_flags;
template <typename E, typename A<E>::type> struct is_typed_flags {
  class Wrap {
  public:
    constexpr operator E() {}
  };
};
}
template <typename E>
constexpr typename o3tl::typed_flags<E>::Wrap operator|(E, E) {
  __PRETTY_FUNCTION__;
}
enum SfxSlotMode { NONE, PROPGET };
namespace o3tl {
template <>
struct typed_flags<SfxSlotMode> : is_typed_flags<SfxSlotMode, 0x1ffffbfL> {};
class {
public:
  short nGroupId;
} a{(short)(NONE | PROPGET)};
}

