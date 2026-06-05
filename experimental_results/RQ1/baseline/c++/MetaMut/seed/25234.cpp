
struct S{friend void f(){}}; //lexical scope of S, only discoverable via ADL, no 
                             //argument, so not discoverable at all yet
void f(); //make f visible to ordinary qualified lookup
int main()
{
  void f(); //declaration should match the declaration at namespace scope, instead       
            //hides the previous declaration at namespace scope
  f(); 
} 

//open question whether a local scope declaration is enough to make a class scope 
//friend function visible to ordinary lookup
//works in clang not in gcc
struct S{friend void f(){}};
int main()
{
  void f();
  f();
}
