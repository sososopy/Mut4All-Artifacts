
#include <iostream>

#define CHECK(expect, actual) std::cout << "EXPECT:" << expect << "\tACTUAL:"<<actual << std::endl
extern bool newCalled;

void testNewWithoutAssignment() {
	newCalled = false;
	new char;
	CHECK(1, newCalled);
}

static char* somechar;
void testNewWithAssignmentToStatic() {
	newCalled = false;
	somechar = new char;
	CHECK(1, newCalled);
}


char *p;
void testNewWithAssignment() {
	newCalled = false;
	p = new char;
	CHECK(1, newCalled);
}

int main() {
	testNewWithoutAssignment();
	testNewWithAssignmentToStatic();
	testNewWithAssignment();
	return 0;
}

