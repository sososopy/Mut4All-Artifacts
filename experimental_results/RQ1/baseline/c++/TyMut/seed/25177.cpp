
#pragma once
#include <string>
extern thread_local std::string gFeelingLucky;



#include "a.h"
thread_local std::string gFeelingLucky = "Lucky";



#include "a.h"
#include <iostream>

int main() {
std::cout << "I'm feeling " << gFeelingLucky << '\n';
}


