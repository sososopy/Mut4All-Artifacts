
struct S
{
    int a;
    static int S::*typeMembers[] = {
        &S::a,
    };
};
