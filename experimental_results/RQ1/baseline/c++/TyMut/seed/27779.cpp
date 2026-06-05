

int* fun()
{
    struct data {
        void *ptr;
    } __attribute__((packed)) var;

    return (int*)(var.ptr);
}

