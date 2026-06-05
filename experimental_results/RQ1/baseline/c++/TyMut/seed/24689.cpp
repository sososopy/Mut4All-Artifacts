
class A {};
template <typename T> class Handle : A {
public:
  operator T &();
};

class JSAtom {};
class PropertyName : public JSAtom {};
template <typename K, typename, int> class InlineMap {
public:
  void remove(const K &);
};
class DefinitionSingle;
template <class Map> struct AtomThingMapPtr {
  Map *operator->();
};
template <typename AtomThingMapPtrT> class D : public AtomThingMapPtrT {};

struct B : AtomThingMapPtr<InlineMap<JSAtom *, DefinitionSingle, 0> > {};
class TokenStream;
class ParseNode {};
struct Definition : ParseNode {
  enum Kind {};
};
class FullParseHandler {
public:
  typedef ParseNode *Node;
};
struct C {};
template <typename> struct ParseContext : C {
  bool define(TokenStream &, Handle<PropertyName *>, FullParseHandler::Node,
              Definition::Kind);
  D<B> lexdeps;
};
template <>
bool ParseContext<FullParseHandler>::define(TokenStream &,
                                            Handle<PropertyName *> name,
                                            ParseNode *, Definition::Kind) {
  lexdeps->remove(name);
}


