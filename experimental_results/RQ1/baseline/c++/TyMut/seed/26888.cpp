
template<typename T>
struct A{ 
	int B; 
};
template<typename T>
void B(){
}
template<typename T> 
void C(){
	A<T> x;
	x.B < 2;
}
int main() {
    return 0;
}
