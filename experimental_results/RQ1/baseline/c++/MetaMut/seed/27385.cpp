
void good1(){
	auto t = [](auto v)
		{
		static_assert (!requires {v*10;});
		//return v*10;
		};
	void * ptr=nullptr;
	t(ptr);
	}

template <class T>
void good2(){
	auto t = [](T v)
		{
		static_assert (!requires {v*10;});
		};
	void * ptr=nullptr;
	t(ptr);
	}

template <class T>
void bad1 (){
	auto t = [](auto v)
		{
		static_assert (requires {v*10;});
		};
	void * ptr=nullptr;
	t(ptr);
	}

template <class T>
void bad2 (){
	auto t = [](auto v)
		{
		static_assert (requires {v*10;});
		};
	T * ptr=nullptr;
	t(ptr);
	}

int main()
{
good1 ();
good2 <void*> ();
bad1 <void*> ();
bad2 <void*> ();
return 0;
}
