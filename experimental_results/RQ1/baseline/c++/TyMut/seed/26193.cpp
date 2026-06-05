typedef struct {
  typedef enum { X, Y } A;
  typedef struct { } B;
  struct C { };
} D;

void foo (D) {}
void foo (D::A) {}
void foo (D::B) {}
void foo (D::C) {}
