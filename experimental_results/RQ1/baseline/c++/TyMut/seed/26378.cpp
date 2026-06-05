
	export module bla;

	extern "C++" inline void fun()
	{
		void oops();  // error: block-scope extern declaration 'void oops()' not permitted in module purview
	}

