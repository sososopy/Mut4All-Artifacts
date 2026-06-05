
#include "gcc-plugin.h"
#include "cp/cp-tree.h"

int plugin_is_GPL_compatible;

void type_callback (void* gcc_data, void*)
{
  tree t = (tree)gcc_data;
  tree binfo = TYPE_BINFO (t);
  if (binfo && BINFO_N_BASE_BINFOS (binfo) > 0) {
    tree base_binfo = BINFO_BASE_BINFO (binfo, 0);
    tree bt = BINFO_TYPE (base_binfo);
    if (bt) {
      type_as_string (bt, 0);
    }
  }
}


extern "C" {

int
plugin_init(struct plugin_name_args   *, struct plugin_gcc_version *)
{
  register_callback ("a", PLUGIN_FINISH_TYPE, type_callback, nullptr);
  return 0;
}


} // extern "C"
