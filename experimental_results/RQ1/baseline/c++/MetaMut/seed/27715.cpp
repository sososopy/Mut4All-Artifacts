
 class out_of_range : public logic_error
 {
 public:
   explicit out_of_range(const string& __arg) _GLIBCXX_TXN_SAFE;
 #if __cplusplus >= 201103L
   explicit out_of_range(const char*) _GLIBCXX_TXN_SAFE;
 #endif
   virtual ~out_of_range() _GLIBCXX_USE_NOEXCEPT;
 };

