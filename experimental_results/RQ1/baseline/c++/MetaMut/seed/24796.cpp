
namespace std
{
  class exception
  {
    virtual ~exception() throw();
  };
}
namespace std __attribute__((__visibility__("default")))
{
  template <typename _CharT, typename _Traits = char_traits<_CharT>, typename _Alloc = allocator<_CharT>>
  class basic_string;
  typedef basic_string<char> string;
}
namespace boost
{
  class exception
  {
  };
}
namespace __cxxabiv1
{
  extern "C" void *__dynamic_cast(const void *__src_ptr, const __class_type_info *__src_type, const __class_type_info *__dst_type, ptrdiff_t __src2dst);
  inline std::string diagnostic_information_impl(boost::exception const *be, std::exception const *se, bool with_what)
  {
    if (!be)
      be = dynamic_cast<boost::exception const *>(se);
  }
}

