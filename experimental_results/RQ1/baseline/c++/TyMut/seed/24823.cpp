
  struct get_answer {};

  template <class Query, class Value>
  struct with : Query {
    Value value;
  };

  int main() {
    with w1{get_answer(), 42}; // works

    with w2(get_answer(), 42); // fails
  }

clang and msvc accept this code. gcc-trunk rejects it. See https://godbolt.org/z/KvGjn47f9.

If I change the definition of the `with` class template to the following, the code compiles:

  template <class Query, class Value>
  struct with {
    Query query;
    Value value;
  };

