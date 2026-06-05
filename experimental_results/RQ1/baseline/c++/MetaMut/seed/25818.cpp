
class cmw_user_input_data_member_flags { 
public: 
    enum E : short { 
        headers = 0x1, 
        middlefile = 0x2, 
        choices = 0x4 
    }; 
    constexpr cmw_user_input_data_member_flags() noexcept {}; 
protected: 
    E val{0}; 
};
