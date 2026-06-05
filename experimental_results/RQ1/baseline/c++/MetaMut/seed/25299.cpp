
typedef struct A { alignas (32) int c; } A;

#define Assert(e) static_assert (e, #e)

typedef alignas (1) A A1_1;
Assert (alignof (A1_1) == alignof (A));

typedef A alignas (1) A1_2;
Assert (alignof (A1_2) == alignof (A));

typedef A A1_3 alignas (1);
Assert (alignof (A1_3) == alignof (A));
