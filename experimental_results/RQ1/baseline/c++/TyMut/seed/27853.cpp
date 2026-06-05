
static void
dump_call_expr_args (cxx_pretty_printer *pp, tree t, int flags, bool skipfirst)
{
  tree arg;
  call_expr_arg_iterator iter;
 
  pp_cxx_left_paren (pp);
  FOR_EACH_CALL_EXPR_ARG (arg, iter, t)
    {
      if (skipfirst)
        skipfirst = false;
      else
        {
          dump_expr (pp, arg, flags | TFF_EXPR_IN_PARENS);
          if (more_call_expr_args_p (&iter))
            pp_separate_with_comma (pp);
        }
    }
  pp_cxx_right_paren (pp);
}

