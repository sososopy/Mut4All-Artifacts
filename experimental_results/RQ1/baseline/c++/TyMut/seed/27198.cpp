
#include <initializer_list>

  template<typename _Tp>
    concept default_initializable =
      requires
      {
	_Tp{};
	(void) ::new _Tp;
      };

static_assert( default_initializable<std::initializer_list<int>> );

