
struct QArrayData
{
    static void free(void *);
    __attribute__((malloc(QArrayData::free))) static void *allocate();
};

