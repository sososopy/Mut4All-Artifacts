

#include <cstdint>

template<std::size_t port>
class Port
{
public: //Declarations
  enum class PinMode
  {
    Input = 0x0,
    Output = 0x1,
    Alternate = 0x2,
    Analog = 0x3
  };

  template<uint8_t nPin, PinMode mode>
  class Pin
  {

  };

  template<uint8_t nPin>
  class Pin<nPin, PinMode::Output>
  {
  public: //Declarations
    enum class OutputSpeed : uint32_t
    {
      Low,
      Medium,
      Fast,
      High
    };

    enum class PullMode : uint32_t
    {
      None = 0x0,
      PullUp = 0x1,
      PullDown = 0x2
    };

  public: //Methods
    void setOutputSpeed(OutputSpeed const ospeed) volatile;
    void setPullMode(PullMode const ppm) volatile;

    void set() volatile
    {
      reinterpret_cast<Port<port> volatile*>(port)->m_BSRR |= static_cast<uint16_t>(0x1) <<nPin;
    }
    void reset() volatile;

    bool getOutputState() volatile;
  }; //END OutputPin

public: //Registers
  uint32_t m_BSRR;
}; //END Port

int main()
{
  auto pin = Port<0>::Pin<5, Port<0>::PinMode::Output>();
  pin.set();
}
