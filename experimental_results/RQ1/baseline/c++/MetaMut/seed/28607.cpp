
template <typename> constexpr bool IsDestructible{};
template <typename T>
constexpr bool IsTriviallyDestructible = IsDestructible<T>;
template <typename T> class Optional {
  ~Optional() requires(!IsDestructible<T>);
  ~Optional() requires(!IsTriviallyDestructible<T>);
};
template <typename, typename ErrorT> class Result {
  using ErrorType = ErrorT;
  Optional<ErrorType> m_error;
};
class SocketError class UDPSocket {
  Result<UDPSocket, SocketError> u16ResultUDPSocketSocketError
