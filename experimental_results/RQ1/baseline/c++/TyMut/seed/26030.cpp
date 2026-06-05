
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

template <class zomg> 
class T {
        vector<int> v{2,6,4,1,7,1}; // #1, fails
	static int comp(int x, int y) 
	{return x < y;} 
public: 
	void sort_me() {sort(v.begin(), v.end(), comp);} 
	void print_me() {for (auto x : v) cout << x << endl;}
}; 


int main()
{
  T<int> t;
	t.sort_me();
	t.print_me();
}

