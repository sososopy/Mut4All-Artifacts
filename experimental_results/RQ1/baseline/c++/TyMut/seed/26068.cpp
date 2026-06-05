

template<typename>struct StringyInterface
{};
template<typename>struct StringyImplementation;
template<>struct StringyImplementation<int>: StringyInterface<int>
{};
template<typename A>struct Stringy
{
  const StringyInterface<A>& verifyConforms =
    StringyImplementation<A>();
};
void
fn1()
{
  Stringy<int>().verifyConforms;
}
