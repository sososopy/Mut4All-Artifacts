
//A.cpp:
export module A;

import <vector>;

std::vector<int> ReturnVector() {
	std::vector<int> result;
	return result;
}


//B.cpp:
export module B;

import A;

