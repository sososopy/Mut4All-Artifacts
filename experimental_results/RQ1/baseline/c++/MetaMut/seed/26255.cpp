#include <string>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <time.h>

using namespace std;

typedef std::map<std::string, int> TItemMap;
typedef std::pair<std::string, int> TItemEntry;
typedef std::map<std::string, int>::iterator TItemMapIter;

int main(int argc, char const *argv[])
{
	const char **n, *item_names[] = {"A", "B", "C", NULL};
	std::map<std::string, int> items;

	for (n = item_names; *n != NULL; n++) {
		items.insert(TItemEntry(*n, 0));
	}

	srand(time(NULL));

	for(int i=0; i<1000; i++) {
		items[item_names[rand() % 3]] += 1;
	}

	for(TItemMapIter g = items.begin(); g!=items.end(); ++g) {
		printf("'%10s': %3d\n", g->first.c_str(), g->second);
	}

	return 0;
}