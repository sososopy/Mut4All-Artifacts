
int f2() = delete[""];
int f3() = delete{""};
int f3() = delete"";

And this compiles without error!

int f() = delete[{'a'"";
int i = f();
