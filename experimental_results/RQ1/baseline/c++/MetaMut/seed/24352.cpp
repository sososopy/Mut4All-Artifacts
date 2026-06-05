
int main()
{ 
   int* pod_type = new int(23);

   for (int i = 0; i < 10000; i++)
     pot_type[i] = 0xDEADC0DE;
 
   delete pod_type; 
   return 0;
}

