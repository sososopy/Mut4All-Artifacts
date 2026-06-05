
#include <cassert>
int main(int argc, char ** argv){
	int ret=0;

    typedef char ch_t;
        assert( sizeof(unsigned char) == 1); //right
        assert( sizeof(ch_t) == 1 ); //right
        assert( sizeof(unsigned ch_t) == 1); //the only one right

    typedef signed char sch;
        assert( sizeof(unsigned char) == 1); //right
        assert( sizeof(sch) == 1 ); //right
        assert( sizeof(unsigned sch) == 4); //wrong, which should be 1

    typedef short sint;
        assert( sizeof(unsigned short) == 2 ); //right
        assert( sizeof(sint) == 2 ); //right
        assert( sizeof(unsigned sint) == 4); //wrong, which should be 8

    typedef long lint;
        assert( sizeof(unsigned long) == 8); //right
        assert( sizeof(lint) == 8 ); //right
        assert( sizeof(unsigned lint) == 4); //wrong, which should be 8

    typedef long long llint;
        assert( sizeof(unsigned long long) == 8); //right
        assert( sizeof(llint) == 8 ); //right
        assert( sizeof(unsigned llint) ==4); //wrong, which should be 8


	return ret;
}
