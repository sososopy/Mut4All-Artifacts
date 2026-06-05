
class ValueType {
  operator int();
  int m_ID;
};
class ValueTypeEnum {
  static constexpr ValueType doubleval = v;
};
template <int> class ValueTypeInfo;
int main() { ValueTypeInfo<ValueTypeEnum::doubleval> }
