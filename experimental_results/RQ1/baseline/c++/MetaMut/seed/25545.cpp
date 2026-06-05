
int ieq(int a, int b) {
	if (a == b)
		return 0;
	else return 1; 
}

int main(int argc, char *argv[])
{
	// where simple-type-specifier is type-name
		typedef int T;
		int i = T(1);
		i = T();
		return ieq(sizeof i, sizeof T());
}
