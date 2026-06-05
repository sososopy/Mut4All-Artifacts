
#ifdef __cplusplus
extern "C" {
#endif

struct foo {
	int a;
};

static inline void
initialize_foo(void *data)
{
	struct foo *foo = data;

	foo->a = 1;
}

#ifdef __cplusplus
}
#endif

int
main(int argc, const char *argv[])
{
	struct foo my_foo;

	initialize_foo(&my_foo);

	return (0);
}
