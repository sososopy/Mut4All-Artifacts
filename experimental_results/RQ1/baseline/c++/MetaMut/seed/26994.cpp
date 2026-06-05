int main() {
  int *p=static_cast<int *>(0),&r=*p;
  return !&r;
}

