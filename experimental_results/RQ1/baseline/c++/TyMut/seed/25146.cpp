
struct A {
  unsigned depth : 1;
};
template <typename> struct Writer_Endpoint;
template <unsigned long> struct B;
struct C {
  struct D {
    template <typename> static Writer_Endpoint<B<0>> *__test();
    typedef decltype(__test<int>()) type;
  };
  D::type operator->();
};
enum Participant_Id { NYSE_MKT };
using participant_id = Participant_Id;
struct F {
  virtual void on_inner_remove(unsigned long, A const &);
};
template <typename> struct O : F {
  void on_inner_remove(unsigned long, A const &);
};
template <typename Logger> struct G {
  template <typename T> void insert(T p1) { logger_->insert(p1); }
  Logger *logger_;
};
template <typename Logger, typename T> void operator<<(Logger p1, T p2) {
  p1.insert(p2);
}
struct H;
struct I {
  H *operator->();
};
struct J {
  using size_type = int;
};
template <unsigned long queue_size> struct Writer_Endpoint<B<queue_size>> : J {
  template <typename, typename, typename...> void emplace();
  struct K;
  template <typename> size_type next_index();
};
template <unsigned long queue_size> struct Writer_Endpoint<B<queue_size>>::K {
  K(Writer_Endpoint *, size_type);
};
template <unsigned long queue_size>
template <typename, typename, typename...>
void Writer_Endpoint<B<queue_size>>::emplace() {
  size_type a = next_index<int>();
  K(0, a);
}
struct H {
  C __trans_tmp_4;
  template <typename T> void insert(T const &);
  decltype(__trans_tmp_4) wep_;
};
template <typename T> void H::insert(T const &) { wep_->emplace<T, int>(); }
template <typename> struct L {
  template <typename T> void insert(T const &);
  I backend_;
};
template <typename Tag> template <typename T> void L<Tag>::insert(T const &p1) {
  backend_->insert(p1);
}
G<L<int>> b, c;
template <typename T_ENCODER>
void O<T_ENCODER>::on_inner_remove(unsigned long, A const &p2) try {
  b << p2.depth;
} catch (...) {
}
struct M;
template <class = int> struct UT_Merger;
template <> struct UT_Merger<> : O<M> {
  UT_Merger() {}
};
template <unsigned long> struct N {
  int const *process();
  N(F p1, int, participant_id, int) : handler_(p1) {}
  void process(int const &, int const &, unsigned short);
  void process(int const &, A const &, unsigned short);
  unsigned char expected_detail_level_;
  F &handler_;
};
template <unsigned long TBookDepth> int const *N<TBookDepth>::process() {
  process(0, 0, 0);
  process(0, *reinterpret_cast<A *>(0), 0);
}
template <unsigned long TBookDepth>
void N<TBookDepth>::process(int const &, int const &, unsigned short) try {
  c << expected_detail_level_;
} catch (...) {
}
template <unsigned long TBookDepth>
void N<TBookDepth>::process(int const &, A const &p2, unsigned short) {
  handler_.on_inner_remove(0, p2);
}
void Synsip_Book_ut_channel_info_TestTestBody() {
  F h;
  N<0> d(h, 0, NYSE_MKT, 0);
  d.process();
}


