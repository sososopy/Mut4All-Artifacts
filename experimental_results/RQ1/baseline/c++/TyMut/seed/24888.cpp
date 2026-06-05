

template <typename>
concept foo = false;

template <typename>
concept bar = true;

template <typename>
struct corge {};

template <foo T>
struct corge<T> {};

template <bar T>
struct corge<T> {
  using foobar = int;
};



module;



export module module1;

export template<class T>
using corge_alias = corge<T>::foobar;


export module module2;
import module1;

struct foobaz{};

using quux = corge_alias<foobaz>;
