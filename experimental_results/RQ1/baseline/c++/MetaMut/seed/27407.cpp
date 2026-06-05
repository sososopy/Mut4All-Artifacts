Changed between 20190609 and 20190616 :


$ cat z1.cc
struct S { int a = 2147483647 + 1; };
struct { S b[2][1][1][1]; } c;


$ cat z2.cc
#include <limits.h>
struct S { int a = INT_MAX + 1; };
struct { S b[2][1][1][1]; } c;


$ g++-11-20201213 -c z1.cc
z1.cc:1:31: warning: integer overflow in expression of type 'int' results in '-2147483648' [-Woverflow]
    1 | struct S { int a = 2147483647 + 1; };
      |                    ~~~~~~~~~~~^~~
z1.cc:2:29:   in 'constexpr' expansion of 'c.<unnamed struct>::<constructor>()'
z1.cc:2:29: internal compiler error: Segmentation fault
    2 | struct { S b[2][1][1][1]; } c;
      |                             ^
0xc8f5ff crash_signal
        ../../gcc/toplev.c:327
0x67c3e8 unshare_constructor(tree_node*)
        ../../gcc/cp/constexpr.c:1527
0x68a51a cxx_eval_vec_init_1
        ../../gcc/cp/constexpr.c:4575
0x681d76 cxx_eval_vec_init
        ../../gcc/cp/constexpr.c:4599
0x681d76 cxx_eval_constant_expression
        ../../gcc/cp/constexpr.c:6584
0x6883c8 cxx_eval_store_expression
        ../../gcc/cp/constexpr.c:5406
0x6811ca cxx_eval_constant_expression
        ../../gcc/cp/constexpr.c:6226
0x680737 cxx_eval_constant_expression
        ../../gcc/cp/constexpr.c:6609
0x680b8c cxx_eval_constant_expression
        ../../gcc/cp/constexpr.c:6284
0x680cde cxx_eval_constant_expression
        ../../gcc/cp/constexpr.c:6295
0x680d70 cxx_eval_constant_expression
        ../../gcc/cp/constexpr.c:6788
0x67f58f cxx_eval_call_expression
        ../../gcc/cp/constexpr.c:2701
0x680f5a cxx_eval_constant_expression
        ../../gcc/cp/constexpr.c:6117
0x683eab cxx_eval_outermost_constant_expr
        ../../gcc/cp/constexpr.c:7126
0x686e7f maybe_constant_init_1
        ../../gcc/cp/constexpr.c:7578
0x6ea7b6 expand_default_init
        ../../gcc/cp/init.c:1998
0x6ea7b6 expand_aggr_init_1
        ../../gcc/cp/init.c:2101
0x6ec475 build_aggr_init(tree_node*, tree_node*, int, int)
        ../../gcc/cp/init.c:1835
0x6cb20d build_aggr_init_full_exprs
        ../../gcc/cp/decl.c:6803
0x6cb20d check_initializer
        ../../gcc/cp/decl.c:6964
---------------------------------------------------------
The master branch has been updated by Jason Merrill <jason@gcc.gnu.org>:

https://gcc.gnu.org/g:6b7d53a25933f4aed71d6d5134e971bd995f8973

commit r11-6320-g6b7d53a25933f4aed71d6d5134e971bd995f8973
Author: Jason Merrill <jason@redhat.com>
Date:   Tue Dec 22 16:40:37 2020 -0500

    c++: Fix constexpr array ICE [PR98332]
    
    The element initializer was non-constant, so its CONSTRUCTOR element ended
    up NULL, so unshare_constructor crashed trying to look at it.  This patch
    fixes this in two places: First, by returning when we see a non-constant
    initializer; second, by not crashing on NULL.
    
    gcc/cp/ChangeLog:
    
            PR c++/98332
            * constexpr.c (unshare_constructor): Check for NULL.
            (cxx_eval_vec_init_1): Always exit early if non-constant.
    
    gcc/testsuite/ChangeLog:
    
            PR c++/98332
            * g++.dg/cpp0x/constexpr-overflow3.C: New test.
---------------------------------------------------------
The releases/gcc-10 branch has been updated by Jason Merrill <jason@gcc.gnu.org>:

https://gcc.gnu.org/g:f5cea9c5c4f8cce36963973e432a8575ef9ffd63

commit r10-9169-gf5cea9c5c4f8cce36963973e432a8575ef9ffd63
Author: Jason Merrill <jason@redhat.com>
Date:   Tue Dec 22 16:40:37 2020 -0500

    c++: Fix constexpr array ICE [PR98332]
    
    The element initializer was non-constant, so its CONSTRUCTOR element ended
    up NULL, so unshare_constructor crashed trying to look at it.  This patch
    fixes this in two places: First, by returning when we see a non-constant
    initializer; second, by not crashing on NULL.
    
    gcc/cp/ChangeLog:
    
            PR c++/98332
            * constexpr.c (unshare_constructor): Check for NULL.
            (cxx_eval_vec_init_1): Always exit early if non-constant.
    
    gcc/testsuite/ChangeLog:
    
            PR c++/98332
            * g++.dg/cpp0x/constexpr-overflow3.C: New test.
---------------------------------------------------------

