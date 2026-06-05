
class String
{
public:
	String();
};

class Value
{
public:
	operator const String&();
	operator int() const;
};

class Concat
{
public:
	Concat();
	Concat& operator+(const String&);
	Concat& operator+(const char&);
};


int main()
{
	Value v;
	Concat concat;
	concat = concat + v + 'A';
}