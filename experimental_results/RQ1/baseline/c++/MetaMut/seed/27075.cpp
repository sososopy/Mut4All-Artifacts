
template <class T, class U>
concept bool Same =
  __is_same_as(T, U);

template <class T, class U>
concept bool ExplicitlyConvertible() { return
  Same<T, U> ||
  requires(T&& t) {
    static_cast<U>((T&&)t);
  };
}

template <class T>
concept bool Constructible() { return
  requires {
    T{ };
  };
}

template <class T, class U>
concept bool Constructible() { return
  ExplicitlyConvertible<U, T>() ||
  requires (U&& u) {
    T{ (U&&)u };
  };
}

template <class T, class U, class V, class...Args>
concept bool Constructible() { return
  requires (U&& u, V&& v, Args&&...args) {
    T{ (U&&)u, (V&&)v, (Args&&)args... };
  };
}

template <class, class...>
constexpr bool core_constructible() { return false; }

template <class T, class...Args>
  requires Constructible<T, Args...>() // ERROR HERE
constexpr bool core_constructible() { return false; }


