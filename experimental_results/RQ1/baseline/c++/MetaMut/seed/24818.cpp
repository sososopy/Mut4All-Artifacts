
#include <algorithm>
#include <cassert>
#include <concepts>
#include <functional>
#include <ranges>
#include <tuple>
#include <type_traits>

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif

namespace std::execution {
  template <class Env, class Query, class... Args>
  concept __has_query =
    requires (const Env& __env, Args&&... __args) {
      __env.query(Query(), std::forward<Args>(__args)...);
    };

  // specialization for key/value pairs
  template <class Query, class Value>
  struct prop {
    [[no_unique_address]] Query __query;
    [[no_unique_address]] Value __value;

    [[nodiscard]] constexpr const Value & query(Query) const noexcept {
      return __value;
    }
  };

  template <class Query, class Value>
  prop(Query, Value) -> prop<Query, unwrap_reference_t<Value>>;

  template <class Env>
  struct __ref;

  template <class Env>
  struct __ref<Env&> {
    Env& __env;

    constexpr __ref(reference_wrapper<Env> __r) noexcept : __env(__r) {}

    template <class Query, class... Args>
      requires __has_query<Env, Query, Args...>
    constexpr decltype(auto) query(Query __q, Args&&... args) const
      noexcept(noexcept(__env.query(__q, std::forward<Args>(args)...))) {
      return __env.query(__q, std::forward<Args>(args)...);
    }
  };

  template<class Slot, size_t N>
  struct __reader {
    friend auto __counted_flag(__reader<Slot, N>);
  };

  template<class Slot, size_t N>
  struct __writer {
    friend auto __counted_flag(__reader<Slot, N>) {}
    static constexpr size_t __n = N;
  };

  template<class Slot, auto Tag, size_t NextVal = 0>
  consteval auto __counter_impl() {
    constexpr bool __counted_past_value = requires(__reader<Slot, NextVal> __r) {
      __counted_flag(__r);
    };

    if constexpr (__counted_past_value) {
      return __counter_impl<Slot, Tag, NextVal + 1>();
    } else {
      return __writer<Slot, NextVal>().__n;
    }
  }

  template<class Slot, auto Tag = []{}, auto Val = __counter_impl<Slot, Tag>()>
  constexpr auto __counter = Val;

  template<class...> struct __list;

  template <class, size_t>
  struct __ignore {
    constexpr __ignore(auto&&) {}
    auto query(auto) const = delete;
  };

  template <class Env>
  using __wrap = conditional_t<is_reference_v<Env>, __ref<Env>, Env>;

  template <class Child, size_t Counter>
  using _as_base_ = conditional_t<Counter == 0, __wrap<Child>, __ignore<Child, Counter>>;

  template <class Parent, class Child, size_t Counter = __counter<__list<Parent, Child>>>
  using _as_base = _as_base_<Child, Counter>;

  // utility for joining multiple environments
  template <class... Envs>
  struct env : _as_base<env<Envs...>, Envs>... {
    template <class Query, class... Args>
    constexpr decltype(auto) __get_1st() const noexcept {
      constexpr bool __flags[] = {__has_query<Envs, Query, Args...>...};
      constexpr size_t __idx = ranges::find(__flags, true) - __flags;
      auto __tup = tie(static_cast<const __wrap<Envs>&>(*this)...);
      return get<__idx>(__tup);
    }

    template <class Query, class... Args>
      requires (__has_query<Envs, Query, Args...> ||...)
    constexpr decltype(auto) query(Query __q, Args&&... args) const
      noexcept(noexcept(__get_1st<Query, Args...>().query(__q, std::forward<Args>(args)...))) {
      return __get_1st<Query, Args...>().query(__q, std::forward<Args>(args)...);
    }
  };

  template <class... Envs>
  env(Envs...) -> env<unwrap_reference_t<Envs>...>;
} // std::execution

// Test code:
template <size_t>
struct get_value_t {
  auto operator()(const auto& env) const noexcept -> decltype(env.query(*this)) {
    static_assert(noexcept(env.query(*this)));
    return env.query(*this);
  }
};

template <size_t I>
inline constexpr get_value_t<I> get_value{};

int main() {
  using namespace std::execution;

  // can create an environment out of a query and a value
  auto env1 = prop(get_value<0>, 42);
  auto val = get_value<0>(env1);
  assert(val == 42);

  // can store a value by reference:
  int i = 42;
  auto env2 = prop(get_value<0>, std::ref(i));
  int& j = get_value<0>(env2);
  ++j;
  assert(i == 43);

  // Can create an env from several envs with duplicates.
  // Queries are resolved by asking the nested envs first to last.
  auto env3 = env(prop(get_value<0>, 42),
                  prop(get_value<1>, 43),
                  prop(get_value<1>, 44),
                  prop(get_value<0>, 45),
                  prop(get_value<0>, 46));
  assert(get_value<0>(env3) == 42);
  assert(get_value<1>(env3) == 43);

  // nested envs can be held by reference also:
  auto env4 = env(prop(get_value<1>, 42), std::cref(env2));
  assert(get_value<0>(env4) == 43);
  assert(get_value<1>(env4) == 42);
}

