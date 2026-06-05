

struct data
{
 virtual ~data() = default;
};

void test(std::shared_ptr<data> b)
{
 auto dpc = std::dynamic_pointer_cast<data>(b);
}
