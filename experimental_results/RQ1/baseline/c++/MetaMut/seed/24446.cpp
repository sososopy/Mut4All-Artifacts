
struct _pthread_fastlock
{
  long int __status;
  int __spinlock;

};
typedef struct _pthread_descr_struct *_pthread_descr;
__extension__ typedef long long __pthread_cond_align_t;
typedef struct
{
  struct _pthread_fastlock __c_lock;
  _pthread_descr __c_waiting;
  char __padding[48 - sizeof (struct _pthread_fastlock)
   - sizeof (_pthread_descr) - sizeof (__pthread_cond_align_t)];
  __pthread_cond_align_t __align;
} pthread_cond_t;


struct S{
    pthread_cond_t x;
    /* g++ 7.3: error: no matching function for call to 'pthread_cond_t::pthread_cond_t(<brace-enclosed initializer list>)'
     * g++ 4.5.3: error: could not convert '{{0, 0}, 0, "", 0}' to 'pthread_cond_t'
     */
    S() : x (PTHREAD_COND_INITIALIZER) {}
};


// Now without the parens:
struct S{
    pthread_cond_t x;
    /* g++ 7.3: Success!
     * g++ 4.5.3: error: could not convert '{{0, 0}, 0, "", 0}' to 'pthread_cond_t'
     */
    S() : x PTHREAD_COND_INITIALIZER {}
};




struct S{
    struct inner{
        char A[1];
    } x;
    /* could not convert '{{0}}' to 'S::inner' */
    S() : x {{0}} {};
};


struct R{
    struct inner{
        char __padding[1];
    } x;
    /* error: no matching function for call to 'R::inner::inner(<brace-enclosed initializer list>)' */
    R() : x ({""}) {};
};


struct R{
    struct inner{
        char __padding[1];
    } x;
    // OK
    R() : x {""} {};
};

