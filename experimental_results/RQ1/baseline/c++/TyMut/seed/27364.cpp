
    module;
    # 1 "" 1 
    namespace std {
    template<class >
    struct char_traits;
    }
    typedef struct {} mbstate_t;
    namespace std {
    template<>
    struct char_traits<char> {
    mbstate_t state_type;                     
    };
    template<typename _CharT, typename = char_traits<_CharT>>
    class basic_string_view {
    public: basic_string_view(const _CharT* ){};
    };
    using string_view = basic_string_view<char>;                      
    }
    # 3 "" 2
    export module hello;
    export void greeter(std::string_view){}

