
enum class Pig { OINK };

struct Hog {
  using enum Pig;
  Hog(Pig) { }
};

template <unsigned>
void pen() {
  (void)Hog(Hog::OINK);
}

void pen() {
  pen<0>();
}
