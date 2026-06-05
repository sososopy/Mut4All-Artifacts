
namespace _ {
template <typename T> class NullableValue {
public:
  T operator*();
  NullableValue(T);
};
}
template <typename T> class Maybe {
public:
  Maybe(T t) : ptr(t) {}
  template <typename Func> auto map(Func f) -> Maybe<decltype(f(T()))> {
    return f(*ptr);
  }
  _::NullableValue<T> ptr;
};
template <typename T> class A {
public:
  T *operator->();
};
typedef int uint64_t;
class B {
public:
  long getId();
};
class C {
  struct D;
  A<D> impl;
  friend class ParsedSchema;
};
class ParsedSchema {
  Maybe<ParsedSchema> findNested() const;
  ParsedSchema(ParsedSchema, C);
  C *parser;
  B getProto() const;
  int raw;
};
class F {
public:
  Maybe<uint64_t> lookup(uint64_t, int);
  ParsedSchema get(uint64_t);
};
struct C::D {
  F compiler;
};
Maybe<ParsedSchema> ParsedSchema::findNested() const {
  int name;
  parser->impl->compiler.lookup(getProto().getId(), name)
      .map([this](uint64_t childId) {
        return ParsedSchema(parser->impl->compiler.get(childId), *parser);
      });
}
