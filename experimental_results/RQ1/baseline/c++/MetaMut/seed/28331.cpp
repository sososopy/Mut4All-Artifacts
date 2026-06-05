
struct S {
  struct Prefs {
    struct {
      int i = j;
      int j = 42;
    } p;
    void Load();
  };
};

void S::Prefs::Load() {
  *this = {};
};
