

namespace NS1 { namespace NS2 {
class Thing {};
} // NS2
using NS2::Thing;
} // NS1


module;

#include "clean.hpp"

export module modA;

export namespace NS1 {
using ::NS1::Thing;
namespace NS2 {
} }



#if 0
#include "clean.hpp"
#else
import modA;
#endif

using NS1::Thing;
using namespace NS1::NS2;
Thing thing;
