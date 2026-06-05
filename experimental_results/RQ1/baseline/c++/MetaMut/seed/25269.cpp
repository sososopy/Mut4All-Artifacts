
#define PORTX (*(volatile unsigned char *)(0x1B))

constexpr volatile unsigned char * testRef(volatile unsigned char & x) {
	return &x;
}
volatile unsigned char* addr = testRef(PORTX); /// OK

constexpr volatile unsigned char* addr2 = testRef(PORTX); /// error: 
   /// reinterpret_cast from integer to pointer
