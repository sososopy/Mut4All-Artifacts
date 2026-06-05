
int container[] = {};
template <class foo>
void bar() {
  for (int &v : container) { }
}

