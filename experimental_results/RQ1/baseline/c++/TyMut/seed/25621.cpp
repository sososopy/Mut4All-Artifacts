
template <typename T> struct X                   { typedef T type; };
template <typename T> struct X<T[0]>             { typedef T type; };
template <typename T, unsigned S> struct X<T[S]> { typedef T type; };
template <typename T> struct X<T[]>              { typedef T type; };

template <typename, typename> struct is_same { enum { value = false }; };
template <typename T> struct is_same<T, T>   { enum { value = true  }; };

static_assert(is_same<X<int>::type, int>::value, "oops");
static_assert(is_same<X<int[]>::type, int>::value, "oops");
static_assert(is_same<X<int[1]>::type, int>::value, "oops");
static_assert(is_same<X<int[0]>::type, int>::value, "oops"); // fails


template <typename T> struct Y                   { enum { value = -2 }; };
template <typename T> struct Y<T[0]>             { enum { value =  0 }; };
template <typename T, unsigned S> struct Y<T[S]> { enum { value =  S }; };
template <typename T> struct Y<T[]>              { enum { value = -1 }; };

static_assert(Y<int>::value == -2, "oops");
static_assert(Y<int[]>::value == -1, "oops");
static_assert(Y<int[1]>::value == 1, "oops");
static_assert(Y<int[0]>::value == 0, "oops"); // fails





template <typename T>
int test(T*) { return 0; }

template <typename T, unsigned I>
char test(T(&)[I]) { return 1; }

int *i;
int j[10];

int a = test(i);
int b = test(j);              // fails; ambiguous call
int c = test((int[]){1,2,3});
int d = test("hello");        // fails; ambiguous call

struct Z {
  int *a;
  int b[1];
  int c[0];
} z;

static_assert(is_same<decltype(test(z.a)), int>::value, "oops");
static_assert(is_same<decltype(test(z.b)), char>::value, "oops"); // fails; ambiguous
static_assert(is_same<decltype(test(z.c)), char>::value, "oops"); // fails (*)

