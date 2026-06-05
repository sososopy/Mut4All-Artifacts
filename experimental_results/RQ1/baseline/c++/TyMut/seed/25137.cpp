
namespace Glib {
class ObjectBase {
protected:
  virtual ~ObjectBase();
};
class A : virtual public ObjectBase {};
class B : virtual public ObjectBase {};
}
namespace Gtk {
class C : Glib::A {};
class D : public Glib::B {};
class TreeViewColumn : C, D {
  ~TreeViewColumn();
};
TreeViewColumn::~TreeViewColumn() {}
}

