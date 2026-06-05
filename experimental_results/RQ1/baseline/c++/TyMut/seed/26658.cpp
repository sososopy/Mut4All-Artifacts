
#include <iostream>
#include <climits>
#include <cstdlib>
using namespace std;
int main() {
	cout << strtoul("-2", nullptr, 10) << endl;
	cout << ULONG_MAX << endl;
	cout << strtoull("-2", nullptr, 10) << endl;
	cout << ULLONG_MAX << endl;
	return 0;
}
