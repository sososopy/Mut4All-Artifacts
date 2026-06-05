
#include <utility>
#include <type_traits>

template<class F> concept NodeFunctionType = std::invocable<std::remove_reference_t<F>, int>;
template<class T> concept ExtracterType = requires { typename T::I_am_an_extracter; };

template<class T = int>
struct NodeMaker {
  template<class... Args>
  static constexpr auto create_node(Args&&... args) { return new T(std::forward<Args>(args)...); }

  template<NodeFunctionType DataMaker> requires (!ExtracterType<DataMaker>)
  static constexpr auto create_node(DataMaker&& data_maker) { return create_node(); } // line 13

  template<ExtracterType DataMaker> requires (!NodeFunctionType<DataMaker>)
  static constexpr auto create_node(DataMaker&& data_maker) { return create_node(); } // line 16

  void do_something() {
    const auto target = create_node(0);  //this does not work in gcc
  }
};

int main() {
  const auto target = NodeMaker<>::create_node(0); //but this works
}
