

extern "C" void * blah_1( void * );  //ok

extern "\x43" void*blah_2(void*);    //ok

extern"\x43"void*blah_3(void*);      //<-- error only here with -std=c++0x and g++4.7.3
                                     //    no error with g++4.6.3

int main() {

}
