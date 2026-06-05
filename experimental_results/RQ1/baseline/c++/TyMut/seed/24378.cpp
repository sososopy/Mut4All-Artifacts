
#include <ostream>
#include <streambuf>
#include <sstream>
#include <vector>
#include <string>
#include <ios>
#include <initializer_list>
#include <uchar.h>
#include <wchar.h>
typedef std::vector<std::u16string> Vu16S;
typedef std::basic_ostringstream<char16_t, std::basic_stringbuf::int_type/*, std::allocator<char16_t>*/ > u16ostringstream;
typedef std::basic_ostringstream<char32_t, std::basic_stringbuf::int_type/*, std::allocator<char32_t>*/ > u32ostringstream;

