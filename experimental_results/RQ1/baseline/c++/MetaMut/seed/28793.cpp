
float HexFloat16ToFloat(const unsigned char* value) {
  unsigned int sign = (static_cast<unsigned int>(value[1]) & 0x80) << 24U;
  unsigned int exponent = (((static_cast<unsigned int>(value[1]) & 0x7c) >> 2U) + 112U)
                      << 23U;
  unsigned int mantissa = ((static_cast<unsigned int>(value[1]) & 0x3) << 8U |
                       static_cast<unsigned int>(value[0]))
                      << 13U;

  unsigned int hex = sign | exponent | mantissa;
  float* hex_float = reinterpret_cast<float*>(&hex);
  return *hex_float;
}

