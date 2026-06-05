

#include <cstdint>
#include <cstdio>
#include <cstddef>

using namespace std;

typedef struct {
        int32_t i32;
        int64_t i64;
} DefaultStruct;

typedef struct {
        int32_t i32;
        int64_t i64 __attribute__((aligned(8)));
} AlignedStruct;

int main(void)
{
        DefaultStruct s;
        printf( "DefaultStruct: offset=%d, struct align=%d, member align=%d, type align=%d\n", 
                offsetof(DefaultStruct,i64), alignof(s), alignof(s.i64), alignof(int64_t) );


        AlignedStruct s2;
        printf( "AlignedStruct: offset=%d, struct align=%d, member align=%d, type align=%d\n",
                offsetof(AlignedStruct,i64), alignof(s2), alignof(s2.i64), alignof(int64_t) );

        return 0;
}


