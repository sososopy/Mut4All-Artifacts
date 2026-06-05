
//#include <QTCore>
#include <stdio.h>
#pragma hdrstop
#define min(a, b)  (((a) < (b)) ? (a) : (b))

int main(int argc, char *argv[])
{
   FILE *f;
   int pos;
   const int bufSize = 10;
   int length;
   int piece;
   int i;
   int ret;
   int chSw = 0;
   const char *fileName = argv[1];
   char *buf;
   if (argc < 1) printf("USAGE:  02space.exe <filename>\nReplaces every symbol \\0x00 by space\n");
   f = fopen(fileName,"rb+");
   if (!f)
   {
       printf("Can not open file %s",argv[1]);
       return -1;
   }
   fseek(f,0,SEEK_END);
   length = ftell(f);
   rewind (f);
   ret = 0;
   buf = new char[bufSize];
   for(piece = (int)(min(bufSize,length)); length > 0;
         length -= piece,piece = (int)(min(bufSize,length)))
   {
     ++ret;
     pos = ftell(f)  ;
     fread(buf,piece,1,f);
     chSw = 0;
     for (i = 0; i < piece; ++i)
     {
         if (buf[i] == '\x00')
         {
             buf[i] = ' ';
             chSw = 1;
         }
     }

     if (chSw)
     {

       fseek(f,pos,SEEK_SET);
       fwrite(buf,piece,1,f);
//       fseek(f,pos,SEEK_SET);
//       fread(buf, piece, 1,f);
     }
   }

   delete [] buf;
   fclose(f);
   return 0;
}
