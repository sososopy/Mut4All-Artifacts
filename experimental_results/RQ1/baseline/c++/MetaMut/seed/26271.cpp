
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	int i=0;
	vector<int> v(10,1);
	for(int i : v)
		cout << i << " ";
			
	for(int i=0; i<5;i++)
	for(int i=0; i<5;i++)
		cout << i << " ";
	
	
	return 0;
}
