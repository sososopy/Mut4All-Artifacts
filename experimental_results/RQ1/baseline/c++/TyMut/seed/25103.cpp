
extern int& x;
int& x = *(new int);
int main() { return !(&x); }
// Comment out the following line for expected operation
extern int& x;

