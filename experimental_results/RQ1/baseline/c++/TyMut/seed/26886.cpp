
    struct B { };

    struct A { 
      explicit A (A const&);
               A (B const&);
    };  

    int main () {
      A x = B {}; // legal
    }   

