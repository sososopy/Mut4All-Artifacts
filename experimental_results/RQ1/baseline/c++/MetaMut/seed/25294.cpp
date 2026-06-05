

class x {};

template<class, class>
class Member {};

class Object {
	Member<x, x> str = Member<x, x>(); // <- line 7
};

int main()
{
}


