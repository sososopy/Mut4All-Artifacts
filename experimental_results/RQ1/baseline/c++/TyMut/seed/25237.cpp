
// valid in C++11 & okay: struct { int a; } s1, s2 = { s1 }; 
struct { int a; } s1, s2 = { s1, 0 };
