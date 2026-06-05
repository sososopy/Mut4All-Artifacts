
template <class type>
class Silly {
 public:
  Silly(type *value) {
    data_ = value;
  }
  operator type*() const {
    return data_;
  }
  template <class other>
  operator Silly<other>() const {
    return Silly<other>(data_);
  }
 private:
  type *data_;
};

int main() {
  Silly<int> a(nullptr);
  delete a;
}

