
namespace std
{
  struct source_location
  {
    struct __impl
    {
      const char* _M_file_name;
      const char* _M_function_name;
      unsigned _M_line;
      unsigned _M_column;
    };
    const __impl* _M_impl = nullptr;
  };
}
const void *ptr = __builtin_source_location ();

