
#ifdef NOWARN
#define __packed
#else
#define __packed __attribute__((packed));
#endif
     
struct Addr
{    
    int s_addr;
};   
struct R1
{    
    Addr addr;                                                                                                                                                                                                                                                                    
} __packed;
     
const Addr& ipAddress(const R1 &r) { return r.addr; }
     
void foo()
{    
    R1 r1;
    (void)ipAddress(r1).s_addr;
}

