
typedef struct {
  struct {
    int x;
  };
  union {
    int y;
    float z;
  };
} S;

void foo(void)
{
  [[maybe_unused]] S a = {
    .x = 1,
    .y = 0
  };
}
