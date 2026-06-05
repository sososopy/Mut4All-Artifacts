
class A {
public:
  int GetLen();
};
class B {
  A s_MDSPartIDStr;
  void FillLoadPartitionInfo();
};
void B::FillLoadPartitionInfo() { char a[s_MDSPartIDStr.GetLen()] = ""; }

