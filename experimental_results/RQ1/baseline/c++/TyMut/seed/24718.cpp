
struct foo 
{ 
  foo(int){} 
}; 

struct bar 
{ 
  explicit bar(int){} 
}; 

void f(foo){} 
void f(bar){} 

int main() 
{ 
  f({0}); 
} 

