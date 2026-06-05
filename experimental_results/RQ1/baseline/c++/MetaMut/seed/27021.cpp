
void* operator new[] (unsigned long, void *p) { return p; }

void foo (void)
{
    char c [1];
    new int [__SIZE_MAX__];
    new (c) int [__SHRT_MAX__][__SHRT_MAX__];
}

