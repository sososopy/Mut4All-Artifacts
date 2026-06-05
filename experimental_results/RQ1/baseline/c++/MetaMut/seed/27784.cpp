

struct s {
  int f;
};

int x(struct s s) {
  return s->f;
}

int x(struct s *s) {
  return s.f;
}


