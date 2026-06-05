
void foo ()
{ 
  0 && ({ goto L1; 0; });
 L1:;
}
