
struct dummy{
	int int_param;
};

inline void Modify_Object(dummy &object){
	object.int_param=0;
}

template <bool is_learning=true> void Templated_Function(){
	const dummy R{0};
	Modify_Object(R);
}

int main(){
	Templated_Function();
}
