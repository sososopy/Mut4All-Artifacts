
static tree
cp_common_type (tree t1, tree t2)
{
  if (SCOPED_ENUM_P (t1) || SCOPED_ENUM_P (t2))
    {
      if (TYPE_MAIN_VARIANT (t1) == TYPE_MAIN_VARIANT (t2))
        return build_type_attribute_variant (t1, attributes);
      else
        return NULL_TREE;
    }

