
class class1; 

namespace ns1 { 

class class2; 
class class3; 
}

ns1::class3 *func(ns1::class2 * cfg, const class1 * def); 

namespace ns1 { 

class class4 { 
#ifdef WORKAROUND
friend ns1::class3 *(::func)(ns1::class2 * cfg, const class1 * def);
#else
friend class3 *(::func)(class2 * cfg, const class1 * def); 
#endif
}; 
}
