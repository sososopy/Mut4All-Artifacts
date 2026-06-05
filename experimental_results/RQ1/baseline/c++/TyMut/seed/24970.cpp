
struct tree2;
struct tree_vector2
{
  tree2 *elts[1];
};
struct tree2
{
  struct
  {
    tree_vector2 vector;
  } u;
};
int
const_with_all_bytes_same (tree2 *val)
{
  int i;
  const_with_all_bytes_same ((val->u.vector.elts[i]));

  return 1;
}
