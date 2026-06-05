

#include <cstdint>
#include <array>

template<typename T>
struct DefaultInstanceOf { static constexpr T value = T(); };

template<typename T>
constexpr T DefaultInstanceOf<T>::value;
	
enum class BasicDataTypeId_t : uint16_t
{
	None = 0,
	INTEGER16 = 0x0003,
};


struct DataTypeX_t
{
private:
	uint32_t mBitSize;
public:
	constexpr uint32_t BitSize() const { return mBitSize; }
	constexpr DataTypeX_t(const uint32_t & bitSize) : mBitSize(bitSize) {};
};



struct DataTypeBasicX_t : public DataTypeX_t
{
	constexpr DataTypeBasicX_t() : DataTypeX_t(50) {};
};

	
struct DataTypeBasicArrayX_t : public DataTypeBasicX_t
{
};

struct BaseDataTypes
{
	static constexpr const auto & ARRAY_OF_INT = DefaultInstanceOf<DataTypeBasicArrayX_t>::value;
};

constexpr const DataTypeX_t & dtarr10 = BaseDataTypes::ARRAY_OF_INT;
constexpr const size_t bitsz10 = dtarr10.BitSize();

int main()
{
  while (1);
}

