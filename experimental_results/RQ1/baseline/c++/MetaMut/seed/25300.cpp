
struct str {
  int a;
  char s[];
};
void fn1() { (struct str){1, ""}; }
