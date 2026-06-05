
typedef unsigned int u32;

struct IO { u32 reg; };

constexpr IO* io = (IO*) 0x40000000;

inline constexpr u32 addr(u32& reg) {
	return reinterpret_cast<u32>(&reg);
}

struct IOTraits {
	static constexpr u32 addr1 = reinterpret_cast<u32>(&io->reg); // works fine
	static constexpr u32 addr2 = addr(io->reg); // compilation error
};
