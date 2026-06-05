
struct AAA { };
struct BBB: virtual AAA { ~BBB (); };


BBB::~BBB() { }
int main() { }


#include "z.h"
struct CCC: BBB {   ~CCC (); };
CCC::~CCC () { }
