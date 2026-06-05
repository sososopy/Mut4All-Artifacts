
#include <atomic>
struct Node { Node* next; };
void Push(std::atomic<Node*>& head, Node* node)
{
    node->next = head.load();
    while(!head.compare_exchange_weak(node->next, node))
        ;
}

