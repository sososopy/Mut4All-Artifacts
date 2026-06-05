
struct c1
{
    ~c1(){}
    c1 operator=(const c1& other)
    {
	return *this;
    }
};

int main( void )
{
    c1 c;
    #pragma omp parallel for lastprivate(c)
    for(int i = 0; i < 10; ++i){}
}


