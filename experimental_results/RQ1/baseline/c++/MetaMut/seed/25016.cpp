struct S { S(int); };
struct T { char n[22]; S s; };
T t[1] = { { "foo", (1) }, };

