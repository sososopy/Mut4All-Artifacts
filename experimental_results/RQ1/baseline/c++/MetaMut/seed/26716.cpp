
struct S {
    int n;
    int a[];
} s;

extern int a[];

enum E {
    e = sizeof s.a,   // rejected in C, accepted in C++
    f = sizeof a      // rejected in both C and C++
};

