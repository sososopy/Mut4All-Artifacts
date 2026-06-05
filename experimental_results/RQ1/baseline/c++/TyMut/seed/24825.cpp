
struct intrusive_list_node {
  void *next;
};

struct dummy {
  void *base;
};

struct thread_info : public dummy, public intrusive_list_node {
  intrusive_list_node node;
};

static thread_info ti;

int main (void) {
  auto thread_info::*MemberNode = &thread_info::node;
  auto node_ptr_1 = &(ti.*MemberNode);
  auto node_ptr_2 = &ti.node;
  return !(node_ptr_1 == node_ptr_2);
}
