
template<class T> void func (T);

template<> void func<int> (int);
template<> void func<int> (int) = delete;

int main () { }

