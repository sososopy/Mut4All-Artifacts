
struct S1 { virtual ~S1(); };
struct S2: S1 {};
struct S3: S2 {};
struct S4: virtual S2 {};
struct S5: S3, S4 {
    S5();
    ~S5();
};
S5::S5() {}

