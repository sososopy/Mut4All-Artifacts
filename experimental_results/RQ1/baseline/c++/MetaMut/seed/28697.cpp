
extern "C" void *memcpy(void *, const void *, unsigned long);
template <int __v>
struct integral_constant {
  static constexpr int value = __v;
};
using false_type = integral_constant<false>;
struct PluginViewBase;
template <bool, typename, typename _Iffalse>
using conditional_t = _Iffalse;
template <typename>
constexpr bool is_const_v = false_type ::value;
namespace WTF {
template <typename ToType, typename FromType>
ToType bitwise_cast(FromType from) {
  ToType to;
  memcpy(&to, &from, sizeof(to));
  return to;
}
}  // namespace WTF
using WTF::bitwise_cast;
bool isOfType___trans_tmp_2;
namespace WTF {
template <typename, typename>
struct TypeCastTraits;
template <typename ArgType>
bool is(ArgType &source) {
  return TypeCastTraits<const int, ArgType>::isOfType(source);
}
template <typename Reference, typename T>
using match_constness_t = conditional_t<is_const_v<Reference>, T, T>;
template <typename, typename Source>
match_constness_t<Source, PluginViewBase> &downcast(Source &source) {
  return static_cast<match_constness_t<Source, PluginViewBase> &>(source);
}
}  // namespace WTF
using WTF::downcast;
using WTF::is;
class PreciseAllocation;
struct HeapCell {
  PreciseAllocation &preciseAllocation() const;
  int &vm() const;
};
struct WeakSet {
  void vm();
};
struct PreciseAllocation {
  static PreciseAllocation *fromCell(void *cell) {
    char *__trans_tmp_1 = bitwise_cast<char *>(cell);
    return bitwise_cast<PreciseAllocation *>(__trans_tmp_1 - halfAlignment);
  }
  void vm() { m_weakSet.vm(); }
  static constexpr unsigned halfAlignment = 2;
  WeakSet m_weakSet;
};
PreciseAllocation &HeapCell::preciseAllocation() const {
  return *PreciseAllocation::fromCell(const_cast<HeapCell *>(this));
}
int &HeapCell::vm() const { preciseAllocation().vm(); }
struct Node {
  virtual bool isPluginElement() { return false; }
};
struct Widget {};
namespace WTF {
template <typename ArgType>
struct TypeCastTraits<const int, ArgType> {
  static bool isOfType(ArgType &source) {
    isOfType___trans_tmp_2 = source.isPluginElement();
    return isOfType___trans_tmp_2;
  }
};
}  // namespace WTF
struct JSHTMLElement {
  Node &wrapped();
};
struct PluginViewBase : Widget {
  virtual HeapCell *scriptObject(HeapCell *);
};
HeapCell pluginScriptObjectFromPluginViewBase_globalObject;
JSHTMLElement pluginScriptObjectFromPluginViewBase_jsHTMLElement;
HeapCell *pluginScriptObjectFromPluginViewBase___trans_tmp_3;
Node &pluginScriptObjectFromPluginViewBase_element =
    pluginScriptObjectFromPluginViewBase_jsHTMLElement.wrapped();
Widget *pluginScriptObjectFromPluginViewBase_pluginWidget;
HeapCell *pluginScriptObjectFromPluginViewBase() {
  if (!is(pluginScriptObjectFromPluginViewBase_element)) return nullptr;
  pluginScriptObjectFromPluginViewBase___trans_tmp_3 =
      downcast<PluginViewBase>(
          *pluginScriptObjectFromPluginViewBase_pluginWidget)
          .scriptObject(&pluginScriptObjectFromPluginViewBase_globalObject);
  return pluginScriptObjectFromPluginViewBase___trans_tmp_3;
}
void pluginElementCustomGetCallData() {
  if (HeapCell *scriptObject = pluginScriptObjectFromPluginViewBase())
    scriptObject->vm();
}
