
class Base
{
public:
    virtual void execute() const = 0;
    virtual ~Base() {}
};

class Pop : public Base
{
public:
    virtual void execute() const { return; }
};

const Pop pop;

int main()
{
    pop.execute();
    return 0;
}
