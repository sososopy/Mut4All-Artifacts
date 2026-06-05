

struct s {
  source_location member = source_location::current();
  int other_member;
  s(source_location loc = source_location::current())
    : member(loc)         // values of member refer to the location of the calling function ([dcl.fct.default])
  {}
  s(int blather) :        // values of member refer to this location
    other_member(blather)
  {}
  s(double)              // values of member refer to this location
  {}
};
void f(source_location a = source_location::current()) {
  source_location b = source_location::current();       // values in b refer to this line
}

void g() {
  f();                   // f's first argument corresponds to this line of code

  source_location c = source_location::current();
  f(c);                  // f's first argument gets the same values as c, above
}

