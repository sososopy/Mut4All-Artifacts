
#include <stdio.h>
#include <stdint.h>

class SimpleFIFO {
public:
  SimpleFIFO() { head = 0; tail = 0; }

public:
  bool isEmpty() { return( head == tail ); }

  uint16_t head;
  uint16_t tail;
};

static SimpleFIFO fifo;

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  printf("isEmpty = %d\n", fifo.isEmpty() );

  while( fifo.isEmpty() ); // -> while( true ) !!!

  // should *never* come here
  printf( "NO WAY ! ! !");
}
