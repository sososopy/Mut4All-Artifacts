
struct Demo
{
private:

  ~Demo()
  {
    __builtin_abort();
  }
};

int main() {
  try
  {
    throw *new Demo;
  }
  catch(const Demo& e)
  {
  }
}
