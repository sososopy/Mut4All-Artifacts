
class myclass{
private:
    int a=0;
public:
    [[deprecated("deprecated")]]
    void deprecate_function() { }
    [[deprecated("deprecated the static")]]
    static void static_deprecate() { }

};

int main()
{
    myclass v;
    v.deprecate_function();
    myclass::static_deprecate();
    v.static_deprecate();
}

