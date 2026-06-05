
struct Object {
  virtual ~Object();
};
struct Configurator : virtual Object {
  virtual void doConfigure() = 0;
};
struct DOMConfigurator : Configurator {
  DOMConfigurator() {}
};
