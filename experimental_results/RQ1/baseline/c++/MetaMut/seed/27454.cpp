
enum class file_type : signed char { none = 0 };

class directory_entry
{
public:
  directory_entry(int);
    
  int _M_path;
  
  file_type _M_type = file_type::none;
};
