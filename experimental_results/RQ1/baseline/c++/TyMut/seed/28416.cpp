
import <memory>;

struct base{int v=0;};

int main()
{
        return std::shared_ptr<base>(new base{})->v;
}

