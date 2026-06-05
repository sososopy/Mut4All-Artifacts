

#include <map>
#include <string>
#include <iostream>

class base
{
	public:

	void from(std::string p_value)
	{
		std::cout << p_value << std::endl;
	}

	void to()
	{
	}

	virtual void from(std::map<std::string, std::string> p_map) = 0;
	virtual void to(std::map<std::string, std::string> p_map) = 0;
};

class derived : public base
{
	public:

	void from(std::map<std::string, std::string> p_map)
	{
	}

	void to(std::map<std::string, std::string> p_map)
	{
	}
};

int main(int, const char**)
{
	derived d;

//	d.to();
//	d.from(std::string("one"));

	return 0;
}
