
typedef void fc1() const; // OK
typedef void frr1() &&; // OK
typedef void fcr1() const &;
using fc2 = void() const; // #4
using frr2 = void() &&; // OK
using fcr2 = void() const &; // #6
