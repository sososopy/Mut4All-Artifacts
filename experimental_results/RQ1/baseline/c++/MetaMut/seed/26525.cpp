

#include <new>

struct Shape {
    void operator delete(Shape *, std::destroying_delete_t);
};

struct Triangle : Shape {};

void Shape::operator delete(Shape *p, std::destroying_delete_t) {
    static_cast<Triangle *>(p)->~Triangle();
    ::operator delete(p);
}

int main() {
    Shape *p = new Triangle;
    delete p;
}

