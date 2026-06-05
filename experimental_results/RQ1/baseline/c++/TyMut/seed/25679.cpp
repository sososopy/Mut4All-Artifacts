
#include <stdio.h>
int main()
{
  int i;
  printf ("int main(){}\n");
  for (i=0;i<70000;i++)
    printf ("__attribute__ ((used, externally_visible)) int ttiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii%i(){}\n", i);
}
