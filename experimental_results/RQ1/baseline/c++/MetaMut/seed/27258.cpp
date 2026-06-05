
#include <cstdlib>
#include <iostream>
#include <functional>

class BraceTest
{
public:
	BraceTest();

	struct BraceTestArrayEntries
	{
		const char *name;
		std::function<void(const char *data)> action;
	};

	void RunAction( const char *actionName );
	void Foo( const char *data );

	int 	_some_data=0;
	BraceTestArrayEntries	_entries[];
};

BraceTest::BraceTest() : _entries { {"FirstAction", [this]( const char *data) {this->Foo( data );} } }
{

}

void BraceTest::RunAction( const char *actionName )
{

}

void BraceTest::Foo( const char *data )
{

}


int main(int argc, char *argv[]) {
	std::cout << "Welcome to the Momentics IDE" << std::endl;

	BraceTest	bt;
	bt.RunAction("FirstAction");

	return EXIT_SUCCESS;
}
