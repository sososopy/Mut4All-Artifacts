
 struct S2 { virtual ~S2(); };
 struct S1 {
     virtual ~S1();
     S2 * f(bool);
 };
 struct S3: S1, S2 {};
 S2 * S1::f(bool b) { return b ? static_cast<S3 *>(this) : nullptr; }

