
//$ cat foo.cc
module;
#include <source_location>
export module foo;

export void log(const std::source_location& loc = std::source_location::current()) {
    // 
}

//$ cat main.cc
import foo;

int main() {
    log();
}

