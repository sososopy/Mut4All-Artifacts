
template<typename T>
long Print(const T& v) {
	return sizeof(T);
}

#include "template.h"

#include <stdio.h>

struct Test {
	bool value;
};

void TestFunc();

int main() {
	fprintf(stderr, "main.cpp: Test: %d   %ld\n", sizeof(Test), Print(Test()));
	TestFunc();
	return 0;
}



#include "template.h"
#include <stdio.h>

struct Test {
	bool value;
	bool value2;
};


void TestFunc() {
	fprintf(stderr, "source.cpp: Test: %d %ld\n", sizeof(Test), Print(Test()));
}

