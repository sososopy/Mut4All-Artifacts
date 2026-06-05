
enum { enum_value };

static void do_nothing_with(void *) { }

int main()
{
  do_nothing_with(static_cast<void *>(static_cast<int>(enum_value)));
}
