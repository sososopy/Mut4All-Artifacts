
class ScriptData {
 public:
  virtual ~ScriptData() { }
  virtual bool HasError() = 0;
};

void operator delete[](void* ptr) throw();
void operator delete(void* ptr) throw();

