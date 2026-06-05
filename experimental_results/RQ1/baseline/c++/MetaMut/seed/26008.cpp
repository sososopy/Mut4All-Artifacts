
void __attribute__ ((__noreturn__)) f();

void __attribute__ ((__noreturn__)) exit(int status) throw();

void exit(int status) throw()
{
    f();
}

