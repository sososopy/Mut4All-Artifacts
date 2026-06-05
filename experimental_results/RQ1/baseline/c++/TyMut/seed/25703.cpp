
struct aa { 
    friend struct cc; 
private: 
    struct bb {}; 
}; 

struct cc : aa::bb {};

