
class Managed {};
class Anchored {
public:
  void release();
  Managed _anchor;
};
template <typename R> void release(R r) {
  static_cast<Anchored *>(r)->release();
}
class Selection : Managed, public Anchored {};
class AppSelectionModel {
  AppSelectionModel() { release(new Selection); }
};

