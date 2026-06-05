
template <>
inline void
freelist<tree_node>::reinit (tree obj ATTRIBUTE_UNUSED)
{
  tree_base *b ATTRIBUTE_UNUSED = &obj->base;
    
#ifdef ENABLE_GC_CHECKING
  gcc_checking_assert (TREE_CODE (obj) == TREE_LIST);
  VALGRIND_DISCARD (VALGRIND_MAKE_MEM_UNDEFINED (obj, sizeof (tree_list)));
  memset (obj, 0, sizeof (tree_list));
#endif

  /* Let valgrind know the entire object is available, but
     uninitialized.  */
  VALGRIND_DISCARD (VALGRIND_MAKE_MEM_UNDEFINED (obj, sizeof (tree_list)));

#ifdef ENABLE_GC_CHECKING
  TREE_SET_CODE (obj, TREE_LIST);
#else
  VALGRIND_DISCARD (VALGRIND_MAKE_MEM_DEFINED (b, sizeof (*b)));
#endif
}

