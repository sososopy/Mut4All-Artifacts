
namespace std {
  struct strong_ordering {
  };
}

namespace DeleteAfterFirstDecl {
  bool operator==(const struct Q&, const struct Q&);
  struct Q {
    friend std::strong_ordering operator<=>(const Q&, const Q&) = default;
  };
}

