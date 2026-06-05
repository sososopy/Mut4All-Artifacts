

struct C{
	static int count;
	C(int, int) {count++;}
};

#define BUG

struct C1{
	static int count;
	explicit C1() {count++;}
	C1(const C1& ) 
	#ifdef BUG
	= delete;
	#else
	{
		cout<<"copy ctor\n";
		count++;
	}
	C1(C1&& ) {
		cout<<"move ctor\n";
		count++;
	}
	#endif
};

struct D{
	D() : c(C(2,3)), cc(3,4) c1(C1()) {}
	C c, cc;
	C1 c1;
};
