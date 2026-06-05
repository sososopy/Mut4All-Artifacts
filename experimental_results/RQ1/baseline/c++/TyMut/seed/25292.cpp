

void fpoke (void *addr, short val, int times, int wait)
{
  volatile unsigned short* FLASH = (volatile unsigned short*)0x20000000;

  *(FLASH + 0) = 0xAA;
  *(FLASH + 1) = 0x55;
  *(FLASH + 0) = 0xA0;
}


