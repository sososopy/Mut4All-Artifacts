
extern int g(int);

void f( int n)
{
	int i;

	i = g( n) & 0x02;

	if (i == 1)
		g( 2);
}

