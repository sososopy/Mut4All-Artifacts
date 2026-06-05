
namespace views {
class Widget;
namespace internal {
class RootView {
// This friend decl creates a bad forward decl of ::views::internal::Widget
  friend class Widget;
};
class NativeWidgetDelegate {
  virtual Widget *AsWidget() = 0;
};
}
class Widget : public internal::NativeWidgetDelegate {
  virtual Widget *AsWidget() override;
};
}

