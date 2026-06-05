
module;
#include <string>
export module animal;
export import :sound;

module;
#include <string> // Comment this to get rid of crash
export module animal:sound;

export std::string makeSound() {
 return "Wroooarrh";
}

#include <cstdio>

import animal;

int main() {
 auto s = makeSound();
 printf("%s", s.c_str());
 return 0;
}
