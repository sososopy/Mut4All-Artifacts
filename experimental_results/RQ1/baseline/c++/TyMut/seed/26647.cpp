
void* operator new[] (unsigned long, void *p) { return p; }

void foo (void)
{
    char c;
    new char [-1];
    new (&c) char [-1];
}

