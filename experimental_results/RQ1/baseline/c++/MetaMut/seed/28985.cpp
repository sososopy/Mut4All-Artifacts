
template<void(&)()>
void h()
{}

void f() noexcept
{}

int main()
{
	h<f>();
}
