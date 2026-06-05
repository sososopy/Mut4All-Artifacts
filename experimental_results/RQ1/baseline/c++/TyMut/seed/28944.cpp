
template <typename, typename> using ElementIdArray = int;
template <int> struct QStaticStringData;
struct GraphModelImpl;
struct __uniq_ptr_impl {
  struct _Ptr {
    using type = GraphModelImpl *;
  };
  using pointer = _Ptr::type;
};
struct unique_ptr {
  using pointer = __uniq_ptr_impl::pointer;
  pointer operator->();
};
struct GraphModel {
  unique_ptr _;
  void buildVisualisations();
} __trans_tmp_2;
template <typename> struct VisualisationsBuilder {
  VisualisationsBuilder(GraphModel, ElementIdArray<int, int>);
  void build() {
    [](auto) {
      enum { Size };
      QStaticStringData<Size> qstring_literal;
    };
  }
};
using NodeVisuals = int;
struct GraphModelImpl {
  NodeVisuals _mappedNodeVisuals;
};
void GraphModel::buildVisualisations() {
  VisualisationsBuilder<int> nodeVisualisationsBuilder(__trans_tmp_2,
                                                       _->_mappedNodeVisuals);
  nodeVisualisationsBuilder.build();
}
