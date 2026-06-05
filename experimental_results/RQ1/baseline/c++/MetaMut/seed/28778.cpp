

struct X {
    struct {
        struct {
            struct {
                struct {
                    struct {
                        struct {
                            struct {
                                int i;
                            };
                        };
                    };
                };
            };
        };
    };
};

int foo (struct X *p)
{
  return p->i;
}
