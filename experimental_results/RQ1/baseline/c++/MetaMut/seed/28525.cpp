
struct foo { int value; };
constexpr foo modify(foo) { return { 0 }; }
template<foo f, bool Enable = f.value>
struct bar
{
    static void run() { }
};
template<foo f>
static void run()
{
    bar<modify(f)>::run();
}
void h()
{
   run<foo{}>();
}
