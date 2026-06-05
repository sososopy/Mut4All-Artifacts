
//write.cpp:

export module write;
export void write(const auto &v) {}


//main.cpp:

import write;
int main() { write(char{}); }

