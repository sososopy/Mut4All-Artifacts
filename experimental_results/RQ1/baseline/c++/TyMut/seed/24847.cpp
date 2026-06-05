
struct Cpu
{
    int op_nop();
};
typedef int(Cpu::*OpCode)();
void f()
{
  new OpCode[256]{&Cpu::op_nop};
}
