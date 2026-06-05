
template<typename T>
struct U;

template<typename S, typename T>
struct U<S (T::*)()>
{};

template<typename S, typename T>
struct U<S (T::*)() &>
{};

template<typename S, typename T>
struct U<S (T::*)() &&>
{};
