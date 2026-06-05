
struct foo{
  foo(const int *new_mac);
};

void g(){
  foo f((int*)((int[1]){ 0xFC}));
}

