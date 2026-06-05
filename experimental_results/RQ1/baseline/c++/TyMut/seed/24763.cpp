struct string {
    string(const char *);
};

struct Json {
    int size() const;
};

const Json &fn1(const Json &obj, const string &key);

int Test1(const Json &arg)
{
    // warning: possibly dangling reference to a temporary [-Wdangling-reference]
    const Json &jv = fn1(arg, "something"); 
    return jv.size();
}

const Json &fn2(const Json &obj);

int Test2(const Json &arg)
{
    // all good
    const Json &jv = fn2(arg);
    return jv.size();
}
