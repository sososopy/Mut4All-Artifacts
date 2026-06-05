
struct SampleModule
{
   explicit SampleModule (int);
};

template < typename >
struct BaseHandler
{
#ifndef BUG
   BaseHandler ();
#endif
   SampleModule module_ { 0 };
};

BaseHandler<int> a;
