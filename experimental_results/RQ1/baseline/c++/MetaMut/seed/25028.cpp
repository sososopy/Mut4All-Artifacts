

    //bad auto

    class c1
    {
     public:
     
     auto& f1()
     {
      return *this;
     }

     auto& f2(auto v)
     {
      return *this;
     }
    };

    int main(int count,const char** arguments)
    {
     c1 a;
     
     a.f1(); //ok
     a.f2(42); //failed
     
     return 0;
    }

