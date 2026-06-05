
class C {

};

namespace N {
C& operator ++(C& obj) { return obj; }
}

C& operator ++(C& obj) { return obj; }

int main()
{
	using N::operator ++;
	C c;
	++c;
}
