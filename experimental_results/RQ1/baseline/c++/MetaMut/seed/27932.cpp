
    #include <cstdio>
    #include <list>

    using namespace std;

    pair<list<int>, list<int>::const_iterator> foo() {
        list<int> l = {1,2,3};
        return {l, l.begin()};
    }

    int main() {
        pair p = foo();
        printf("%i\n", *p.second);
    }
