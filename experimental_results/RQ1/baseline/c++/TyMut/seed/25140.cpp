

    #include <cstdlib>
    #include <cstdio>
    
    enum class state {
        null,
        initialized,
        destroyed,
    };
    
    extern void broken_atexit();
    
    struct global_data {
        state s;
    
        global_data()
            : s(state::null)
        {
            std::puts("delegated constructor");
        }
        global_data(int) 
            : global_data()
        {
            s = state::initialized;
            std::atexit(&broken_atexit);
            std::puts("delegating constructor");
        }
        ~global_data(){
            s = state::destroyed;
        }
    } data(1);
    
    void broken_atexit(){
        if(data.s == state::destroyed){
            std::puts("attempt to use a destroyed object?");
            std::abort();
        }
        std::puts("okay");
    }
    
    int main(){
    }

