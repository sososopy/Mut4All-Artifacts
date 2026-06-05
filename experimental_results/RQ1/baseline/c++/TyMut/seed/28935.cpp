
   typedef long unsigned int size_t;
        typedef struct   {
        }
        __sigset_t;
                                            typedef int (*__compar_fn_t) (const void *, const void *);
        extern __inline __attribute__ ((__gnu_inline__)) void * bsearch (const void *__key, const void *__base, size_t __nmemb, size_t __size,   __compar_fn_t __compar) {
        size_t __l, __u, __idx;
        int __comparison;
        while (__l < __u)     {
           if (__comparison < 0)  __u = __idx;
           else if (__comparison > 0)  __l = __idx + 1;
         }
      }
