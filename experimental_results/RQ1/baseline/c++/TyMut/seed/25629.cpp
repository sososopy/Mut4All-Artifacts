
int i;

constexpr int & iref = i;
constexpr int & irefref = iref;
// works

class A {
        static constexpr int & irefref = iref;
        //                               ^
        // error: field initializer is not constant
};
