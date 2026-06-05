
typedef int vec __attribute__((vector_size(4*sizeof(int))));
void f(vec* a, vec* b, vec* c){
  *c = (*a < *b) && (*b < *a);
}

