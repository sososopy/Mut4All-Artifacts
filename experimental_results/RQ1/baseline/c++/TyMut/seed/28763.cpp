
//Contents of test1.cc:
	extern __thread int thing;
	__thread int thing = 0;

//Contents of test2.cc:
	int main()
	{
		extern __thread int thing;
		return thing;
	}

