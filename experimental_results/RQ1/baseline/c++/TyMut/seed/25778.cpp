
typedef enum { EE } *PtrE;

template <typename T>
char (*foo(T))[13];

char (*foo(void *))[42];

extern char check[sizeof *foo(PtrE(0))];
extern char check[42];


