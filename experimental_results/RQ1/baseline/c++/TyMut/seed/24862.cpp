
struct I {
    const bool b;
    
};
struct O {
   
    I a[2];  
    
    static I const data[2];
    O() : a(data){}
} ; 

I const O::data[2] = {true, false};
