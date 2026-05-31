use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Item;
use syn::ReturnType;
use syn::Type;
use syn::TypeParamBound;
use syn::Stmt;
use syn::Expr;

pub struct Dyn_Star_Trait_Return_330;

impl Mutator for Dyn_Star_Trait_Return_330 {
    fn name(&self) -> &str {
        "Dyn_Star_Trait_Return_330"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut has_trait = false;
                        let mut has_lifetime = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if !trait_bound.path.segments.is_empty() {
                                    has_trait = true;
                                }
                                if trait_bound.lifetimes.is_some() {
                                    has_lifetime = true;
                                }
                            }
                        }
                        if has_trait && has_lifetime {
                            if let syn::Block { stmts: block } = &*item_fn.block {
                                if let Some(stmt) = block.last() {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        let is_coercion = if let Expr::Cast(expr_cast) = expr {
                                            if let Type::Infer(_) = *expr_cast.ty {
                                                true
                                            } else {
                                                false
                                            }
                                        } else {
                                            false
                                        };
                                        if !is_coercion {
                                            let mut new_block = item_fn.block.clone();
                                            if let Some(last_stmt) = new_block.stmts.last_mut() {
                                                if let Stmt::Expr(expr, _) = last_stmt {
                                                    let original_expr = expr.clone();
                                                    let new_expr = parse_quote!(#original_expr as _);
                                                    *expr = new_expr;
                                                }
                                            }
                                            item_fn.block = new_block;
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
        ""
    }
}