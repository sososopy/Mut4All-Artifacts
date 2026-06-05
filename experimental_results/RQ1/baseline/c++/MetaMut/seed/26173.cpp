
struct E {};
int main()
{
    try {
	throw E();
    } catch (E &e) {
    }
    return 0;
}
