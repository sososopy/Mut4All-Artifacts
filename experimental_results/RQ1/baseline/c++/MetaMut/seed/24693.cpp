
template<typename T>
class __attribute((abi_tag("foo"))) test{  };

template class __attribute((abi_tag("foo"))) test<int>;

int main() { }

