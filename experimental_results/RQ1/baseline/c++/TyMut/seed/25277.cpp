

#include <new>
struct failure_case {unsigned long s;};

void break_func(unsigned int s) {
    char temp_buffer[sizeof(failure_case) + s];
    failure_case *f = new (temp_buffer) failure_case;
};
