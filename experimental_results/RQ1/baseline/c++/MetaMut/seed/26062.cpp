
namespace std {
  typedef long unsigned int size_t;    
};
namespace boost { 
  namespace detail {
    struct is_static_visitor_tag { };
    typedef void static_visitor_default_return;
  } 
  template <typename R = ::boost::detail::static_visitor_default_return>
  class static_visitor    : public detail::is_static_visitor_tag {
  public:
    typedef R result_type;
  protected:
    static_visitor() { }    ~static_visitor() { }
  };}
namespace boost
{
  template <class T> struct hash;
  template <class T> void hash_combine(std::size_t& seed, T const& v);
  template <class It> std::size_t hash_range(It, It);
  template <class It> void hash_range(std::size_t&, It, It);
}
namespace boost {
  namespace detail { namespace variant {
      struct variant_hasher: public boost::static_visitor<std::size_t> {
	template <class T>
	std::size_t operator()(T const& val) const {
	  using namespace boost;
	  hash<T> hasher;
	  return hasher(val);
	}      };    }  }}
