
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <pthread.h>

struct list_node_t
{
	list_node_t *next;
	list_node_t *prev;
};

template <typename T, list_node_t T::*inner_list_node>
class list_t
{
	public:
		list_t() { _head.next = _head.prev = &_head; }
		bool is_empty() const { return _head.next == &_head; }
		T* entry(list_node_t &node) const { return &node == &_head ? NULL : (T*)((char*)&node - (char*)_node_offset); }

		void add(T &node)
		{
			_head.next->prev = &(node.*inner_list_node);
			(node.*inner_list_node).next = _head.next;
			(node.*inner_list_node).prev = &_head;
			_head.next = &(node.*inner_list_node);
		}


		static void del(T &node)
		{
            int slot_1 = 10000;
            printf("slot_1 %d\n", slot_1);
			(node.*inner_list_node).next->prev = (node.*inner_list_node).prev;
			(node.*inner_list_node).prev->next = (node.*inner_list_node).next;
			int slot_2 = 20000;
            printf("slot_2 %d\n", slot_2);
		}

	protected:
		static list_node_t const * const _node_offset;
		list_node_t _head;
};
template <typename T, list_node_t T::*inner_list_node>
list_node_t const * const list_t<T, inner_list_node>::_node_offset = &(((T *)0)->*inner_list_node);

template <typename T, list_node_t T::*inner_list_node>
class safe_list_t: public list_t<T, inner_list_node>
{
	public:
		safe_list_t(): _alive(1),_num(0)
	{
		pthread_mutex_init(&_mutex, NULL);
		pthread_cond_init(&_cond, NULL);
	}

		~safe_list_t()
		{
			pthread_cond_destroy(&_cond);
			pthread_mutex_destroy(&_mutex);
		}

		int len()
		{
			return _num;
		}

		void put(T &node)
		{
			pthread_mutex_lock(&_mutex);
			if (_alive)
			{
                this->add(node);
				++_num;
			}
			pthread_mutex_unlock(&_mutex);
			pthread_cond_signal(&_cond);
		}

		T* get()
		{
			T *ret;
			pthread_mutex_lock(&_mutex);
			while (_alive && list_t<T, inner_list_node>::is_empty())
				pthread_cond_wait(&_cond, &_mutex);
			if (_alive)
			{
                ret = this->entry(*list_t<T, inner_list_node>::_head.prev);
                this->del(*ret);
				--_num;
			}
			else
			{
				ret = NULL;
			}
			pthread_mutex_unlock(&_mutex);
			return ret;
		}

	protected:
		pthread_mutex_t _mutex;
		pthread_cond_t _cond;
		int _alive;
		int _num;
};

template <class T>
class worker_t
{
public:
    T *t;
    list_node_t inner_task_list_node;

public:
    worker_t():t(NULL)
    {
        t = new T;
    }
    worker_t(T *tt):t(tt)
    {
    }

    ~worker_t()
    {
        delete t;
    }
 
private:
    worker_t(const worker_t& rhs);
    worker_t& operator=(const worker_t& rhs);
};

int main() {
    safe_list_t<worker_t<size_t>, &worker_t<size_t>::inner_task_list_node>   _serialize_merge_scan_move_list;

    auto * worker_0 = new worker_t<size_t>;
    *worker_0->t = 100;
    _serialize_merge_scan_move_list.put(*worker_0);

    auto * worker_1 = new worker_t<size_t>;
    *worker_1->t = 200;
    _serialize_merge_scan_move_list.put(*worker_1);

    auto* temp_0 = _serialize_merge_scan_move_list.get();
    printf("temp_0->t = %zu\n", *temp_0->t);
    delete temp_0;

    
    auto* temp_1 = _serialize_merge_scan_move_list.get();
    printf("temp_1->t = %zu\n", *temp_1->t);
    delete temp_1;
}

