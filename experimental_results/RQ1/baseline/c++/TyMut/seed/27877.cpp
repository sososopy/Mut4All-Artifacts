
void f();

void b(void* p = (f(), nullptr));

void z()
{
    b();
}
