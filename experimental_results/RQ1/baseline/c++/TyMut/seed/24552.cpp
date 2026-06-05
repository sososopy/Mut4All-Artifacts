
class A {
  static void addWindow();
  static void activateWindow(void *);
};
void A::addWindow() {
  int* action {};
  [action] { activateWindow(action); };
}

