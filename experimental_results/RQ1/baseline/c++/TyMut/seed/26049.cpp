

    case CPP_WSTRING_USERDEF:
      string_len = 3;
    case CPP_STRING16_USERDEF:
    case CPP_STRING32_USERDEF:
      string_len = 5;
    case CPP_UTF8STRING_USERDEF:
      string_len = 4;
      bad_encoding_prefix = true;
    case CPP_STRING_USERDEF:

