

typedef unsigned long size_t;

  struct __sso_string
  {
    struct __str
    {
      const char* _M_p;
      size_t _M_string_length;
      char _M_local_buf[16];
    };

    union {
      __str _M_s;
      char _M_bytes[sizeof(_M_s)];
    };
  };
