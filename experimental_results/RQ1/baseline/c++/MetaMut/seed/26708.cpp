
class CMymy
{
	unsigned char _a;
	unsigned char _b;
public:
	constexpr CMymy() : _a(), _b() {}

	constexpr CMymy(const CMymy &) = default;
	CMymy &operator=(const CMymy &) = default;

} __attribute__((aligned(2)));
