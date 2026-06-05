
class Foo 
{
};
Foo& getfoo();
void foo()
{
        Foo& f = getfoo();
}

class Bar
{
        virtual ~Bar() {}
};
Bar& getbar();
void bar()
{
        Bar& b = getbar();
}