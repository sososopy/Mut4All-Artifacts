
#include <iostream>
using namespace std;
void FuncA(int arg1, int arg2)
{
  int i = 0, j = 0;
  unsigned char noConst = 2;
  const unsigned char index1 = 16;
  const unsigned char index2 = noConst;
  unsigned char temp3[index1][index2] = { 0 };
  for(i = 0; i < index1; i++) {
    for(j = 0; j < index2; j++) {
      printf("%d\t", temp3[i][j]);
    }
    printf("\n");
  }
}
int main()
{
  FuncA(0, 0);
  return 0;
}

