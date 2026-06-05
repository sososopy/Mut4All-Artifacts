
class MemoryMappedRegister {
  public:
  MemoryMappedRegister(const int value) : 
    ptr{reinterpret_cast<char*>(value)} {}
  void write(char v) const { *ptr = v; }
  
  private:
  volatile char* const ptr;
};

const MemoryMappedRegister MyReg(12345);

void set_value(char v) {
  MyReg.write(v);
}

