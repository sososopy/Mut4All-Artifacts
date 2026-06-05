
//mod.cpp:

export module mod;
export template<class T>
concept C = false;


//test.cpp:

import mod;
static_assert(C<int>);

