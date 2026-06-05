
module ;


#define _GLIBCXX_VISIBILITY(V) __attribute__ ((__visibility__ (#V)))

namespace std _GLIBCXX_VISIBILITY(default)
{

}


export  module  hello:format;


module ;


#define _GLIBCXX_VISIBILITY(V) __attribute__ ((__visibility__ (#V)))
#define _GLIBCXX_BEGIN_NAMESPACE_VERSION 
namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION
}


export  module  hello;
export  import  :format;
