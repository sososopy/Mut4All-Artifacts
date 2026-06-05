
#include <stdio.h>

typedef struct 
{ 
 unsigned char  a;     //  1 byte
 unsigned char  b;     //  1 byte
 unsigned char  c;     //  1 byte
 unsigned char  d;     //  1 byte
 unsigned char  e;     //  1 byte
 unsigned char  f;     //  1 byte
 unsigned char  g;     //  1 byte
 unsigned char  h;     //  1 byte
} tsMMP_bytes;         //  8 bytes

typedef struct 
{ 
 unsigned char  a;     //  1 byte
 unsigned short b;     //  2 bytes - skips one byte here
 unsigned char  c;     //  1 byte
 unsigned char  d;     //  1 byte
 unsigned short e;     //  2 bytes - does not skip one byte here
 unsigned char  f;     //  1 byte
} tsMMP_mixed;         //  8 bytes 

typedef struct 
{ 
 unsigned char  a;     //  1 byte
 unsigned char  b;     //  1 byte
 unsigned short c;     //  2 bytes - does not skip one byte here
 unsigned char  d;     //  1 byte
 unsigned short e;     //  2 bytes - skips one byte here
 unsigned char  f;     //  1 byte
} tsMMP_mixed2;        //  8 bytes 

typedef struct 
{ 
 unsigned char  a;     //  1 byte
 unsigned short b;     //  2 bytes - skips one byte here
 unsigned char  c;     //  1 byte
 unsigned short d;     //  2 bytes - skips one byte here
 unsigned char  e;     //  1 byte
 unsigned char  f;     //  1 byte
} tsMMP_mixed3;        //  8 bytes 

typedef struct
{
 unsigned char l;
 char h; 
} tsWord;  // signed word DOES NOT WORK!

typedef struct
{
 unsigned char  a;          //  1 byte
 tsWord         b;          //  2 bytes - works properly
 unsigned char  c;          //  1 byte
 unsigned char  d;          //  1 byte
 tsWord         e;          //  2 bytes - works properly
 unsigned char  f;          //  1 byte
} tsMMP_mixed_struct_word;  //  8 bytes - correctly maps to another linear memory 

// 20201225T0311 : Kemal Akcam : Mixed typedef struct works perfectly while using as memory stencil for memory mapping.
typedef struct
{
 unsigned char a;
 unsigned char b; 
} tsShort;

typedef struct
{
 unsigned char  a;     //  1 byte
 tsShort        b;     //  2 bytes - works properly
 unsigned char  c;     //  1 byte
 unsigned char  d;     //  1 byte
 tsShort        e;     //  2 bytes - works properly
 unsigned char  f;     //  1 byte
} tsMMP_mixed_struct;  //  8 bytes - correctly maps to another linear memory 

class c
{
public:
 c(){};
 ~c(){};

 unsigned char m[16] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                         0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25 };
 
 void printContent(void);
};

void c::printContent(void)
{
 int i;
 printf("Memory content             ");
 for(i=0; i<16; i++) printf("%X ", m[i]);
 printf("\n"); 
 
 printf("tsMMP_bytes                ");
 tsMMP_bytes *pB;
 pB = (tsMMP_bytes *)&m;
 printf("%X %X %X %X %X %X %X %X ", 
         pB->a,
         pB->b,
         pB->c,
         pB->d,
         pB->e,
         pB->f,
         pB->g,
         pB->h);
 printf("\n"); 

 {
 printf("tsMMP_mixed                ");
 tsMMP_mixed *pM;
 pM = (tsMMP_mixed *)&m;
 printf("%X %X %X %X %X %X   <--- 11 is NOT there", 
         pM->a,
         pM->b,
         pM->c,
         pM->d,
         pM->e,
         pM->f);
 printf("\n"); 
 }
 {
 printf("tsMMP_mixed2               ");
 tsMMP_mixed2 *pM;
 pM = (tsMMP_mixed2 *)&m;
 printf("%X %X %X %X %X %X   <--- 15 is NOT there", 
         pM->a,
         pM->b,
         pM->c,
         pM->d,
         pM->e,
         pM->f);
 printf("\n"); 
 }
 { 
 printf("tsMMP_mixed3               ");
 tsMMP_mixed3 *pM;
 pM = (tsMMP_mixed3 *)&m;
 printf("%X %X %X %X %X %X   <--- 11 and 15 is NOT there", 
         pM->a,
         pM->b,
         pM->c,
         pM->d,
         pM->e,
         pM->f);
 printf("\n"); 
 }

 printf("tsMMP_mixed_struct         ");
 tsMMP_mixed_struct *pMS;
 pMS = (tsMMP_mixed_struct *)&m;
  
 printf("%X %X %X %X %X %X   <--- 11 and 15 is there", 
         pMS->a,
         pMS->b,
         pMS->c,
         pMS->d,
         pMS->e,
         pMS->f);
 printf("\n"); 

 printf("tsMMP_mixed_struct_word    ");
 tsMMP_mixed_struct_word *pMWord;
 pMWord = (tsMMP_mixed_struct_word *)&m;
 printf("%X %X %X %X %X %X   <--- 11 and 15 is there",  
         pMWord->a,
         pMWord->b,
         pMWord->c,
         pMWord->d,
         pMWord->e,
         pMWord->f);
 printf("\n"); 
  
 printf("tsMMP_mixed_struct_word    ");
  short int *b = (short int *)&(pMWord->b);
  short int *e = (short int *)&(pMWord->e);
  printf(" corr b e %04X %04X ", *b, *e);
 printf("\n"); 

 printf("tsMMP_mixed_struct_word    ");
  printf("%X %X %X %X %X %X %X %X ", 
         pMWord->a,
         pMWord->b.h,
         pMWord->b.l,
         pMWord->c,
         pMWord->d,
         pMWord->e.h,
         pMWord->e.l,
         pMWord->f);
  printf("\n");
}

int main(int argc, char **argv) 
{
 c *n = new c();
 n->printContent();
 n->~c();

 return 0;
}
