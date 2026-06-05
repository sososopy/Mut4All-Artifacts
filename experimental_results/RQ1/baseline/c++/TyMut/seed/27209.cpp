
namespace clang {
  class DynTypedNode {
  private:
    template <typename T, typename EnablerT = void> struct BaseConverter;
    template <typename T> struct ValueConverter {};
  };
  namespace ast_type_traits {
    using DynTypedNode = ::clang::DynTypedNode;
  }; // end namespace ast_type_traits
}; // end namespace clang

namespace clang {
  namespace ast_type_traits {
    template <> struct DynTypedNode::BaseConverter<int, void> : public ValueConverter<int> {};
  }; // end namespace ast_type_traits
}; // end namespace clang

int main(void) {
    return 0;
}

