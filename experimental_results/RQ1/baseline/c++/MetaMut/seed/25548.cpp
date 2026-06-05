
inline constexpr int popCount(const unsigned long uVal)
{
	return __builtin_popcountll(uVal);
}

int main()
{
	return popCount(45);
]
