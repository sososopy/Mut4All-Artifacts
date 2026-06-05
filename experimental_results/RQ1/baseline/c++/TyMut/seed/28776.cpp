
HOST_WIDE_INT
bytes_in::wi ()
{
  HOST_WIDE_INT v = 0;
  if (const char *ptr = read (1))
    {
      v = *ptr & 0xff;
      if (v & 0x80)
	{
	  unsigned bytes = (v >> 4) & 0x7;
	  v &= 0xf;
	  if (v & 0x8)
	    v |= -1 ^ 0x7;
	  if ((ptr = read (++bytes)))
	    while (bytes--)
	      v = (v << 8) | (*ptr++ & 0xff);
	}
      else if (v & 0x40)
	v |= -1 ^ 0x3f;
    }

  return v;
}

