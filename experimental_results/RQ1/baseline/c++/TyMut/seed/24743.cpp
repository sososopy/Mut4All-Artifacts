
namespace foo {
// no closing brace


module;

#include "header.hpp"

export module honeydew;



void 
declare_module (module_state *module, location_t from_loc, bool exporting_p, tree, cpp_reader *reader)
{
  gcc_assert (global_namespace == current_scope ());

