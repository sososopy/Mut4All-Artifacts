
struct X
{
	struct Y {};
};

struct X::Y xy; /*B*/
struct X::Y; /*A*/

int main()
{
	return 0;
}
