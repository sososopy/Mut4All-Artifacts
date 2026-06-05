
struct Base { };

template <typename>
struct Fixed {
  typedef const char* name;
};

#if BUG
template <typename VT, typename... Fields>
void New(const char* name,
         typename Fixed<Fields>::name... field_names);
#endif

template <typename VT, typename... Fields>
void CreateMetric(const char* name,
                  typename Fixed<Fields>::name... field_names,
                  const Base&) { }


void Fn()
{
  CreateMetric<int, const char*>("abcd", "def", Base());
}
