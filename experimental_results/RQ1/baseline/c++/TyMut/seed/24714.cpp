
template < typename FactoryT > void newFrontendActionFactory (FactoryT *, int * = 0);
int a;

template < typename FactoryT >
void newFrontendActionFactory (FactoryT *, int *)
{
    class A
    {
        void m_fn1 ()
        {
            B (0, 0);
        }
        class B
        {
        public:
            B (FactoryT, int);
        };
    };
    newFrontendActionFactory (&a);
}
