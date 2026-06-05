
class A {};

class B : public virtual A {};

class C : public A {};

int main() {
    B* object = new B();
    B &ref = *object;

    // -Waddress warns here
    bool b = nullptr == dynamic_cast<A*>(&ref);

    // -Waddress doesn't warn anymore
    auto ptr = dynamic_cast<A*>(&ref); 
    bool b2 = ptr == nullptr;

    C* cobject = new C();
    C &cref = *cobject;

    // -Waddress also doesn't warn anymore
    bool b3 = nullptr == dynamic_cast<A*>(&cref);
}

