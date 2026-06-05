
typedef struct a {} A;

void f() {
 if (__builtin_alignof(A))
 ;
}
