
template<class T> T&& declval();

template<typename _To1>
void __test_aux(_To1);

template<typename _From1, typename _To1,
        typename = decltype(__test_aux<_To1>(declval<_From1>()))>
char __test(int);

template<typename, typename>
int __test(...);

enum E {
    x = decltype(__test<E, int>(0))(0);
};

