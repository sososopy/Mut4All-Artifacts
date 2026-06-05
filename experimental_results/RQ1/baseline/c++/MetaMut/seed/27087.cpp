

inline hashval_t
auto_hash::hash (tree t)
{
  if (tree c = PLACEHOLDER_TYPE_CONSTRAINTS (t))
    /* Matching constrained-type-specifiers denote the same template
       parameter, so hash the constraint.  */
    return hash_placeholder_constraint (c);
  else
    /* But unconstrained autos are all separate, so just hash the pointer.  */
    return iterative_hash_object (t, 0);
}

