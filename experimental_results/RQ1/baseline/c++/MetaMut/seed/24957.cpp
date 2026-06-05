


namespace {
template <typename> class unique_ptr {};
class DataContainerBinary;
namespace {
struct BinaryValue;
union FieldValue {
  BinaryValue *binary;
};
struct BinaryValue {
  int length;
  char data[];
};
}
template <typename T, typename Args> unique_ptr<T> make_unique(Args &&);
namespace {
unique_ptr<DataContainerBinary> to_binary(FieldValue field) {
  make_unique<DataContainerBinary>(field.binary->data);
}
}
}

