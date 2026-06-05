
#include <array>

template <typename All, typename Left, typename Rigth>
struct basic_reg {
    union {
        All value;
        struct {
            Rigth lower;
            Left higher;
        } halfs;
    };
};

template <typename Left>
struct basic_reg<Left, Left, void> {
    Left value;
};

template <typename Register, unsigned short Size>
struct basic_registers {
    typedef Register register_t;
    typedef std::array<register_t, Size> registers_t;
    basic_registers() : m_registers{0} {}
    typename registers_t::reference
    operator[](typename registers_t::size_type i) {
        return m_registers[i];
    }

private:
    registers_t m_registers;
};

typedef basic_reg<uint8_t, uint8_t, void> bits_08_register;
typedef basic_reg<uint16_t, bits_08_register, bits_08_register>
    bits_16_register;
typedef basic_reg<uint32_t, bits_16_register, bits_16_register>
    bits_32_register;
typedef basic_registers<bits_32_register, 1> igpr_bits_32_registers;

struct basic_registry_wrapper {
    typedef std::array<uint16_t*, 1> gp_bit16_array;
    typedef std::array<uint32_t*, 1> gp_bit32_array;

    basic_registry_wrapper(igpr_bits_32_registers& registers) {
        m_gpr_bits16_array[0] = &registers[0].halfs.lower.value;
        m_gpr_bits32_array[0] = &registers[0].value;
    }

    uint16_t& ax() { return *m_gpr_bits16_array[0]; }
    uint32_t& eax() { return *m_gpr_bits32_array[0]; }

private:
    gp_bit16_array m_gpr_bits16_array;
    gp_bit32_array m_gpr_bits32_array;
};

int main() {
    igpr_bits_32_registers reg_32;
    basic_registry_wrapper reg_wrap_32(reg_32);

    if (reg_wrap_32.eax() != 0)
        return -1;
    reg_wrap_32.eax() = 0x12345678;
    if (reg_wrap_32.ax() != 0x5678)
        return -1;
    return 0;
}

