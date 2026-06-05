

__attribute__ ((target ("default")))
void foo() { }

__attribute__ ((target ("sse4.2")))
void foo() { }

int main ()
{
  foo();
}

