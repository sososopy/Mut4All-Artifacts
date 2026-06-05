
template <typename T>                                                                                                                                                                                                                   
void Index(int i) {                                                                                                                                                                                                                     
  static int VAR __attribute__((used,section("NEW_SECTION"))) = i;                                                                                                                                                                       }                                                                                                                                                                                                                                       
                                                                                                                                                                                                                                    
template void Index<int>(int);
