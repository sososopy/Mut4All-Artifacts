
extern void f();
extern void h(int, int*);

void g(int m, int *a){
    if (m <= 1) return;
    int j = 0;
    for (int k = m; k < m + 2; k++){
      if(a[k]) j = (a)[k];
    }
    if (j) {
      f();
    }
    int z = m +1;
    _Cilk_spawn h(m+1, a);
    h(z+2, a);
}
