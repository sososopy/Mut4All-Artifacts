
int main()
{
	int x;
	int & y {x};
	return reinterpret_cast<long int>(&y) == 1;
}
