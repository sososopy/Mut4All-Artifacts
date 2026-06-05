

#include <cstdlib>
#include <climits>
#include <cstdio>

int main() {
	int myint = INT_MAX;
	long mylong = LONG_MAX;
	long long myllong = LLONG_MAX;
	printf("%lu\n", sizeof(myint));
	printf("%lu\n", sizeof(mylong));
	printf("%lu\n", sizeof(myllong));
	printf("%d\n", myint);
	printf("%ld\n", mylong);
	printf("%lld\n", myllong);
	printf("%d\n", -abs(myint));
	printf("%ld\n", -abs(mylong));
	printf("%lld\n", -abs(myllong));
	printf("%d\n", -abs(myint));
	printf("%ld\n", -labs(mylong));
	printf("%lld\n", -llabs(myllong));
}

