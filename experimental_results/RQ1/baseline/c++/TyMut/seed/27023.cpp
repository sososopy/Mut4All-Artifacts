

#ifndef SZ
# define SZ 16
#endif
struct Range {
  short B, E;
  Range() : B(0), E(0) {}
};
struct ArrayOfRanges {
  Range LabelRanges[1 << SZ] = {};
};
ArrayOfRanges *AOR = new ArrayOfRanges();

