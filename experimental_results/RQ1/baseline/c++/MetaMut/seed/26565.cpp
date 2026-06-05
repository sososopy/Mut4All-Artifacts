#include <iostream>
#include <string>
#include <map>
#include <vector>

int values(){
	std::cout << "calling values" << std::endl;
	static int idx;
	return idx++;
}
std::string keys(){
	std::cout << "calling keys" << std::endl;
	static std::vector<std::string> str = {"A", "B", "C", "D", "E"};
	static int a;
	return str[(a++)%str.size()];
}
int main(int argc, char **argv){
	std::map<std::string, int> mm;
	mm[keys()] = values();
}

