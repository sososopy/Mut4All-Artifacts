class S {
  using T = int;
  enum class E : T;
};

enum class S::E : S::T { X };

