enum class Type { Pawn };
struct Piece {
  Type type : 4;
};
void foo() {
  switch (Piece().type)
    case Type::Pawn:;
}

