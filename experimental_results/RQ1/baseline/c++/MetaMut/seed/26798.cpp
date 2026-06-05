struct S {
    const int &r1, &r2;

    S ():
    r1 (r2),
    r2 (r1)
    { }
};
