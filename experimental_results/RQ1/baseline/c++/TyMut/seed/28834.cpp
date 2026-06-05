
export  module  format;

export namespace NS
{
void Format ();
}

export  module  hello:check;

export namespace NS {}


export  module  hello;
export import :check;
import  format;

export namespace NS
{
using NS::Format;
}


module  hello;
