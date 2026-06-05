
class myclass
{
public:
   template<typename T_>
   inline explicit myclass(T_ *s)
   {
      char buf[mylib::someclass::some_const]; // this line causes the fault
      // ... do something useful with buf
   }

   ...      
};

// somewhere else:
namespace mylib
{
   class someclass
   {
   public:
      static const uint32_t some_const;
   }
}

