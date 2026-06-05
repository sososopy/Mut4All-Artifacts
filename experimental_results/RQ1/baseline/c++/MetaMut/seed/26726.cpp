
class DenseMap {
public:
  ~DenseMap();
};
const DenseMap &GCMap{};
void foo() { delete &GCMap; }

