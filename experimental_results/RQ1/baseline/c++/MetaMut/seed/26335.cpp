

const unsigned long Size = (1l << 62);

struct Chunk {
  char padding[Size];
  char data;
};

int test6 = __builtin_offsetof(Chunk, data);
