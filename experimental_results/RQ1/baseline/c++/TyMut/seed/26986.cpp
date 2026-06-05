
struct Task {
  struct TaskStaticData {
    constexpr TaskStaticData() {}
  } const &tsd;
  constexpr Task() : tsd(TaskStaticData()) {}
};

Task tasks{Task()};

