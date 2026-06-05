

#include <stdio.h>

struct Class {
    Class(void (*_param)()) : data(_param) {}
    void (*data)();
};

void funUser(void (*test)(int)) {
    test(60);
}

void user(Class& c, int i) {
    (void)i;
    printf("%p data: %p\n", &c, c.data);
}
void probe() {}


int main() {
    static Class instance = { probe };
    //user(instance, 0); // (1)

    funUser([](auto... p) {
        user(instance, p...);
    });
}
