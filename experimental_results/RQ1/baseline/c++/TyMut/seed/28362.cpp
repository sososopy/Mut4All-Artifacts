
struct BaseClass {
    // Commenting out this non-const function out will fix the compilation:
    int baseDevice() { return 1; }
    int baseDevice() const { return 2; }
};

template <class ObjectClass>
struct DerivedClass : BaseClass {};

template <class ObjectClass>
struct TopClass : DerivedClass<ObjectClass> {
public:
    virtual int failsToCompile() const {
        // This should choose to call the const function, but it tries to call
        // the non-const version.
        return BaseClass::baseDevice();                   // error!
        //return this->baseDevice();                      // works
        //return DerivedClass<ObjectClass>::baseDevice(); // works
    }
};

int main() {
    TopClass<int> x; 
}
