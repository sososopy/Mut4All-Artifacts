
   static inline void __iowrite(uint32_t src, uint32_t* dst) { *dst=src; }
   #define iowrite32(val,iomem) __iowrite((uint32_t)(val), (uint32_t*)(iomem))

   static void __miowrite32(void* d, void* s, unsigned count)
   {
      while (count--)
      {
         iowrite32(*((uint32_t*)s), d);
         ++(*((uint32_t**)&s));
         ++(*((uint32_t**)&d));
      }
   }

