use crate::mutator::Mutator;

pub struct Modify_Const_Expr_In_Generic_Call_177;

impl Mutator for Modify_Const_Expr_In_Generic_Call_177 {
    fn name(&self) -> &str {
        "Modify_Const_Expr_In_Generic_Call_177"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct GenericCallVisitor<'a> {
            lifetime: Option<&'a Lifetime>,
        }

        impl<'a> VisitMut for GenericCallVisitor<'a> {
            fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
                if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                    if let Some(last_segment) = path.segments.last_mut() {
                        if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    if let Expr::Block(expr_block) = expr {
                                        let new_stmt: Stmt = if let Some(lifetime) = self.lifetime {
                                            parse_quote! {
                                                let _: &#lifetime ();
                                                let tmp = 3;
                                                tmp
                                            }
                                        } else {
                                            parse_quote! {
                                                let tmp = 3;
                                                tmp
                                            }
                                        };
                                        expr_block.block.stmts.insert(0, new_stmt);
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, expr_call);
            }
        }

        struct LifetimeVisitor<'a> {
            lifetime: Option<&'a Lifetime>,
        }

        impl<'a> Visit<'a> for LifetimeVisitor<'a> {
            fn visit_lifetime(&mut self, i: &'a Lifetime) {
                self.lifetime = Some(i);
            }
        }

        let mut lifetime_visitor = LifetimeVisitor { lifetime: None };
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(sig) = &item_fn.sig.generics.params.iter().find_map(|param| {
                    if let GenericParam::Lifetime(lifetime_def) = param {
                        Some(lifetime_def)
                    } else {
                        None
                    }
                }) {
                    lifetime_visitor.visit_lifetime(&sig.lifetime);
                }
            }
        }

        let mut visitor = GenericCallVisitor {
            lifetime: lifetime_visitor.lifetime,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function calls with constant generic parameters. It modifies the constant expression by introducing a temporary variable and optionally referencing an existing lifetime parameter. This ensures the constant expression resolves correctly while potentially exposing subtle bugs in handling lifetimes and temporaries."
    }
}