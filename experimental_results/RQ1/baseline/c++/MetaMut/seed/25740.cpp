
int main()
{
	int *volatile setAndUsed = new int[1];
	delete[] setAndUsed;
}
