
#include <dlfcn.h>
#include <cassert>
#include <string>
#include <iostream>

void* LoadLib(std::string name)
{
      void* libInstance;
      name = "lib" + name + ".so";
      libInstance = dlopen(name.c_str(), RTLD_NOW);
      if ( ! libInstance ) std::cout << "Loading of dictionary library failed. Reason: " << dlerror() << std::endl;
      return libInstance;
}

bool UnloadLib(void* libInstance)
{
     int ret = dlclose(libInstance);
     if (ret == -1)
     {
        std::cout << "Unloading of dictionary library failed. Reason: " << dlerror() << std::endl;
        return false;
     }
     return true;
}

int main()
{
   void* instance = LoadLib("dll");
   assert(instance != 0);

   assert(UnloadLib(instance));
   std::cout << "DLL unloaded" << std::endl;
}

