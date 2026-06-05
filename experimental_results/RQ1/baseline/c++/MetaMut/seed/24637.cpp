
#include <stdio.h>

int main(void) {
        char buf[10];
        snprintf(buf, 10, "%i", 0);
        printf("buf=%s\n", buf);
        return 0;
}
