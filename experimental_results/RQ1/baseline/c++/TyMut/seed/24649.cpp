
typedef union tree_node *tree;
enum built_in_function { BUILT_IN_ACOS, BUILT_IN_FPCLASSIFY, BUILT_IN_ISFINITE };
struct tree_function_decl {
    __extension__ enum built_in_function function_code : 11;
};
union tree_node {
    struct tree_function_decl function_decl;
};
static tree
convert_arguments (tree fundecl)
{
  switch (((fundecl)->function_decl.function_code))
    {
      case BUILT_IN_ISFINITE:
      case BUILT_IN_FPCLASSIFY:
	return (tree) 0;
    }
  return fundecl;
}


