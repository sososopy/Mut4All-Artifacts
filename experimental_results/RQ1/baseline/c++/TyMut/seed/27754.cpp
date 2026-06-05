
  float testValue = 3.25;
  char* warningAvoider = reinterpret_cast<char *>(&testValue);
  int32_t rawValue = *(reinterpret_cast<int32_t*>(warningAvoider));
