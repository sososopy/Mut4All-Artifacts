
#include <QList>
#include <iostream>

int main(int argc, char** argv) {
  QList<int> list;
  list.append(1);
  list.append(2);
  list.append(3);
  foreach(int x, list) {
    std::cout << x << std::endl;
  }

  return 0;
}

