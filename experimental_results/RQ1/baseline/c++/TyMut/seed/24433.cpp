template<typename T>
struct my_queue {
	void push(T){
	}
	void ice(){
		trav([&](auto &&v){ push(v); });
	}
	template<typename F>
	void trav(F &&f){
		f(T());
	}
};
template class my_queue<int>;

