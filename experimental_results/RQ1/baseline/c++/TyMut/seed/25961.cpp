
class Type {
public:
   int *a = new int[10]{0};
   Type () { }
   Type(const Type  & lt) { }
};
int main () {
   Type lt1;
   Type lt2 (lt1);;
}
