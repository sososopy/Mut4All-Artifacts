
template<typename A, typename B> class First {
  public:
    First() = default;

  private:
    int GetId() const {
      return 1;
    }

    template<typename U, typename V, typename D>
    friend bool operator==(const First<U, V>& lhs,
                           const First<D, V>& rhs);
};
template<typename U, typename V, typename D>
bool operator==(const First<U, V>& lhs,
                const First<D, V>& rhs) {
                             return lhs.GetId() == rhs.GetId();
                           }

int main() {
  First<int, bool> a;
  First<void *, bool> b;

  return a == b;
}

