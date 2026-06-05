
struct STest {
	static int GetVal(int x) {return vals[x];}
	static constexpr int vals[] = { 1,2,3,4,4,4,4 };
};
int main() 
{
    return STest::GetVal(1);
}
