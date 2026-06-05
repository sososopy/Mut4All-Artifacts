namespace std
{
  struct stop_token { };

  template<typename Callback>
    struct stop_callback
    {
      template<typename C>
      stop_callback(stop_token, C&&) { }
    };

template<typename _Callback>
    stop_callback(stop_token, _Callback) -> stop_callback<_Callback>;
}

int main()
{
  std::stop_token tok;
  std::function<void()> f([](){});
  std::stop_callback cb(tok, f);
}

