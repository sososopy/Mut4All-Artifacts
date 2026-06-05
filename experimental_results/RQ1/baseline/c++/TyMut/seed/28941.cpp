
struct link {
  link* next{nullptr};
  link* prev{nullptr};
};

struct list {
  static inline link tail{&tail, &tail};
};

list l;


