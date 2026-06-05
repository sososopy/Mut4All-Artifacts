
template <const char name[]>
class BaseObject {
 virtual const char* GetName() const {
   return name;
 }
};

const char kName[] = "name";
class Object : public BaseObject<kName> {
};

int main() {
 return 0;
}
