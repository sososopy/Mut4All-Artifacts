
struct aaa { virtual ~aaa() {} } ;

struct bbb : public aaa {};

void foo(aaa* aaa) {
    dynamic_cast<bbb*>(aaa);
}
