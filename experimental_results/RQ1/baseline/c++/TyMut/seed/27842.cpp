
struct empty {};

struct has_empty {
	[[no_unique_address]] empty brace_or_equal_initialized{};
};

struct has_value {
	int non_zero = 1;
};


struct pair : has_empty, has_value {};

pair a;
//pair b = pair();





<source>:14:7: internal compiler error: in output_constructor_regular_field, at varasm.c:5207

   14 | pair a;

      |       ^




See it live: https://godbolt.org/z/tvdsmg


The error occurs if you comment out a and uncomment b, as well. `brace_or_equal_initialized` can also be initialized by a regular constructor call, the important part is just that the constructor is explicitly called.
---------------------------------------------------------
The master branch has been updated by Jason Merrill <jason@gcc.gnu.org>:

https://gcc.gnu.org/g:6876b269bc7fe6465fedfed87c31e6175992129f

commit r10-7031-g6876b269bc7fe6465fedfed87c31e6175992129f
Author: Jason Merrill <jason@redhat.com>
Date:   Wed Mar 4 12:08:42 2020 -0500

    c++: Fix [[no_unique_address]] and default mem-init [PR90432]
    
    output_constructor doesn't like two consecutive entries with fields at the
    same position; let's avoid adding the one for the empty field.
    
    gcc/cp/ChangeLog
    2020-03-04  Jason Merrill  <jason@redhat.com>
    
    	PR c++/90432
    	* init.c (perform_member_init): Don't do aggregate initialization of
    	empty field.
    	* constexpr.c (cx_check_missing_mem_inits): Don't enforce
    	initialization of empty field.
---------------------------------------------------------
The releases/gcc-9 branch has been updated by Jason Merrill <jason@gcc.gnu.org>:

https://gcc.gnu.org/g:9af9e004831f8efdfb68c2affea07b17fadd3279

commit r9-8332-g9af9e004831f8efdfb68c2affea07b17fadd3279
Author: Jason Merrill <jason@redhat.com>
Date:   Wed Mar 4 17:30:58 2020 -0500

    c++: Fix [[no_unique_address]] and default mem-init [PR90432]
    
    output_constructor doesn't like two consecutive entries with fields at the
    same position; let's avoid adding the one for the empty field.
    
    gcc/cp/ChangeLog
    2020-03-04  Jason Merrill  <jason@redhat.com>
    
    	PR c++/90432
    	* init.c (perform_member_init): Don't do aggregate initialization of
    	empty field.
    	* constexpr.c (cx_check_missing_mem_inits): Don't enforce
    	initialization of empty field.
---------------------------------------------------------

