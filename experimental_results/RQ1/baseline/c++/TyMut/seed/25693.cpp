
  extern unsigned long ADDR;   

  unsigned long f(){
    const unsigned long* const var=(unsigned long*)(&ADDR);
    const unsigned long retval=var[1];
    return retval;
  }
