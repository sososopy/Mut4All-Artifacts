
namespace std { class type_info {}; }

namespace std { class type_info; }

import "bug_a.ii";

namespace std {
  const type_info* __cxa_exception_type () noexcept;
}
