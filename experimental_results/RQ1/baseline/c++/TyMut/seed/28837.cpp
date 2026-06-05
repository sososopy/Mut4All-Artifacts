
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

struct _node_t {
	_node_t * _next;
};
static void _rebase_ptr(char *&ptr, void *old_base, void *new_base) {
	if ( ptr != 0 ) {
		ptr = (char *)((char *)(ptr) - (char *)(old_base) + (char *)(new_base));
	}
}

static void _rebase_ptr( _node_t *&head, void *old_base, void *new_base ) {
	_rebase_ptr(*(char**)&head, old_base, new_base);
	// error!! value of "head" not changed

	_node_t * node = head;
	while( node != 0 ) {
		_rebase_ptr( *(char**)&(node->_next), old_base, new_base );
		node = node->_next;
	}
}

int main() {
	_node_t *tmp = (_node_t *)calloc(1, sizeof(_node_t));
	_node_t *tmp2 = (_node_t *)calloc(1, sizeof(_node_t));
	memcpy(tmp2, tmp, sizeof(_node_t));
	free(tmp);

	_rebase_ptr(tmp, tmp, tmp2);
	printf("%llx == %llx\n", (long long)tmp, (long long)tmp2);
	free(tmp);
	return 0;
}
