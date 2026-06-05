
extern "C" int printf( const char *, ... );
extern "C" void* memset( void *s, int c, unsigned int n );

template<typename T>
class ArrayRef
{
  private:
    const T* data;
  public:
    ArrayRef( const T& oneElt )
    {
      this->data = &oneElt;
      printf( "ArrayRef's data: %p\n", this->data );
    }
    const T &operator[]( unsigned int index ) const { return data[index]; }
};

class Obj { public: int val; };
class SpecialObj : public Obj {};

int main()
{
  SpecialObj specialObj;
  printf( "&specialObj:     %p\n", &specialObj );

  SpecialObj* pSpecialObj = &specialObj; //Triggers the bug.
  //Obj* pSpecialObj = &specialObj;  //Shows expected behaviour.
  printf( "&pSpecialObj:    %p\n", &pSpecialObj );

  ArrayRef<Obj*> arrayRef( pSpecialObj );

  int someStackArray[500];
  memset( someStackArray, 0xDD, sizeof(someStackArray) );

  Obj* basePtr = arrayRef[0];
  printf( "basePtr:         %p\n", basePtr );

  if( basePtr == pSpecialObj ) //Do we got what we put into arrayRef?
  {
    printf( "Pointer is valid: OK.\n" );
  }
  else
  {
    printf( "Pointer (%p) is corrupt, program will crash!!!! GCC Bug?\n", basePtr );
  }
  basePtr->val = 42;
  return 0;
}

