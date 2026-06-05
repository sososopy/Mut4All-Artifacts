
__inline
__attribute__ ((__gnu_inline__))
int test(int v)
{
	return v;
}

int main(int argc, char **argv)
{
	return test(argc);
}
