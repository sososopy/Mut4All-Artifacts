
struct pair {
  int second;
};
template <typename _InputIterator, typename _Function>
void for_each(_InputIterator, _Function __f) {
  _InputIterator __first;
  __f(*__first);
}
template <typename, unsigned long> struct array { int operator[](long); };
class span {
public:
  template <typename _Tp, unsigned long _ArrayExtent>
  span(array<_Tp, _ArrayExtent>);
};
enum Trans_NS_constants_gb_playerstate_t { ALLIN };
class gamecards {
  void operator==(gamecards);
};
template <int> class gamestate {
protected:
  array<Trans_NS_constants_gb_playerstate_t, 1> m_playerstate;
  void operator==(gamestate);
};
template <int N> class game : gamestate<N>, gamecards {
public:
  game(span, int);
  array<int, 1> payouts() {
    pair __trans_tmp_2;
    for_each(&__trans_tmp_2,
             [&](auto e) { this->m_playerstate[e.second] == ALLIN; });
  }
};
void TestBody() { array<int, 1> cards = game<3>(cards, 0).payouts(); }
