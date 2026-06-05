
class ostream;
template <typename Type>
concept OstreamInsertable = requires(ostream out, Type value) {
  out << value;
};
struct FMT {};
class CSVTabIns {
  template <OstreamInsertable Type> friend void operator<<(CSVTabIns, Type);
};
template <OstreamInsertable Type> void operator<<(CSVTabIns, Type &);
void operator<<(CSVTabIns tab, FMT) {
  tab << 1;
}
