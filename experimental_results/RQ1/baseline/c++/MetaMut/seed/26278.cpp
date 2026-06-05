
template <typename T>
struct ArrayRef {
  constexpr ArrayRef(const T*) {}
};
struct IRBuilder {
  template <typename = void>
  static void CreateConstInBoundsGEP2_32() {
    ArrayRef<void*> IdxList(0);
  }
};
void LLVMBuildStructGEP() { IRBuilder::CreateConstInBoundsGEP2_32(); }
