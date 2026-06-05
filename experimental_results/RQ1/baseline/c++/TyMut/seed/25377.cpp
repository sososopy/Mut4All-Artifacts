

#include <iostream>
using namespace std;

int ticket();

class Manager {

 friend int ::ticket() {  // qualified function name
     return ++Manager::counter;     
 }
 static int counter;
};

int Manager::counter;

int main() {
	
    cout << ticket();
    return 0;
}

