
export  module  format;

export namespace NS
{
void Format ();
}


export  module  hello;
import  format;

export namespace NS
{
using NS::Format;
}

