
void g( int);

struct S
{
	int a;
	int b;

	S & operator = ( const S & that)
	{
		g( that.a);
	}
};

