
typedef unsigned int v4si __attribute__ ((vector_size(16)));
typedef unsigned char v16qi __attribute__ ((vector_size(16)));
extern v16qi x;
v4si &y = x;
