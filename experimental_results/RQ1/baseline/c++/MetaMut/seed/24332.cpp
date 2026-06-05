

static char* table2[10];
template<unsigned size, char*(&table)[size]> void test2() { }
void tester2() { test2<10,table2>(); } // error: 'table2' cannot appear in a constant-expression

