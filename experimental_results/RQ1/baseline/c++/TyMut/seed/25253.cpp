
struct string
{
        string(const char*);
};

template <class T>
struct expression_evaluator
{
        typedef T result_type;
        result_type& declare_parameter(const string& name);
};

template <class T>
void test_declarations()
{
        expression_evaluator<T> evaluator;
        T& param = evaluator.declare_parameter("p1")=T(4);
}

void foo()
{
        test_declarations<unsigned>();
}
