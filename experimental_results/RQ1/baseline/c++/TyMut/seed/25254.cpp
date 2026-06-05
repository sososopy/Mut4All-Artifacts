

struct Object
{
    int key;
};

// Making this a class rather than a template allows it to compile
template <typename Key>
class Foo
{
public:
    // Changing the returning type to const Object* allows it to compile
    auto operator->() const { return &b; }
    
private:
    Object b;
};

// Making this a class rather than a template allows it to compile
template <typename T>
class X
{
protected:
    Foo<int> cont;
    
public:
    int get_val() const
    {
        // error: result of 'operator->()' yields non-pointer result
        return cont->key;

        // Calling operator-> directory compiles fine
        //return cont.operator->()->key;
    }
};

int main()
{
    return 0;
}
