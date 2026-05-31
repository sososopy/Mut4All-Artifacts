use crate::mutator::Mutator;

pub struct Modify_Generic_Const_Expressions_188;

impl Mutator for Modify_Generic_Const_Expressions_188 {
    fn name(&self) -> &str {
        "Modify_Generic_Const_Expressions_188"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    for segment in &mut trait_bound.path.segments {
                                        if segment.ident == "for" {
                                            if let PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                                for arg in &mut args.args {
                                                    if let GenericArgument::Const(expr) = arg {
                                                        if let Expr::Block(expr_block) = expr {
                                                            if let Some(stmt) = expr_block.block.stmts.first_mut() {
                                                                if let Stmt::Expr(Expr::Binary(expr_binary), _) = stmt {
                                                                    if let Expr::Path(ref expr_path) = *expr_binary.left {
                                                                        if expr_path.path.segments.len() == 1 {
                                                                            let ident = &expr_path.path.segments[0].ident;
                                                                            let new_expr: Expr = parse_quote! { #ident * 2 };
                                                                            *stmt = Stmt::Expr(new_expr, None);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies function definitions with a `where` clause that includes generic constant expressions. It modifies these expressions to introduce non-linear mathematical operations, potentially causing evaluation instabilities. The mutation specifically targets the default value of const parameters, changing them to involve operations like multiplication."
    }
}