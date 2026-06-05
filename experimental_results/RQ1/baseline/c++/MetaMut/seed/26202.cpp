
    #include <cstdio>
    #include <cstdint>
    
    struct __attribute__((packed)) Xyz {
        uint32_t seq_no;
        uint64_t end_offset;
    };
    
    struct Mapping {
        void* mem;
        size_t len;
    
        template<class T>
        T* begin() {
            return static_cast<T*>(mem);
        }
    
        template<class T>
        T* end() {
            auto e = reinterpret_cast<uintptr_t>(static_cast<char*>(mem) + len);
            return reinterpret_cast<T*>(e - e % sizeof(T));
        }
    };
    
    int main() {
        Mapping m{reinterpret_cast<void*>(0x40000u), 0x1000u};
        Xyz* beg = m.begin<Xyz>();
        Xyz* end = m.end<Xyz>();
        size_t len = end - beg;
        printf("%p\n", beg);
        printf("%p\n", end);
        printf("%zx\n", len);
    }
