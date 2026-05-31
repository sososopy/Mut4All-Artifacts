use syn::{parse_quote, Item, Stmt, Pat, PatType, Type, Expr, ImplItem, Local, ExprArray};
use crate::mutator::Mutator;

struct Generic_Const_Exprs_Array_Size_Mutation_366;

impl Mutator for Generic_Const_Exprs_Array_Size_Mutation_366 {
    fn name(&self) -> &str {
        "Generic_Const_Exprs_Array_Size_Mutation_366"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Type(pat_type) = &local.pat {
                            if let Some(init) = &local.init {
                                if let Expr::Array(array_expr) = init {
                                    if let Some(elems) = &array_expr.elems {
                                        let new_len = elems.len() + 1;
                                        local.init = Some(Expr::Array(ExprArray {
                                            attrs: array_expr.attrs.clone(),
                                            bracket_token: array_expr.bracket_token,
                                            elems: parse_quote!([(); #new_len]),
                                        }));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Pat::Type(pat_type) = &local.pat {
                                    if let Some(init) = &local.init {
                                        if let Expr::Array(array_expr) = init {
                                            if let Some(elems) = &array_expr.elems {
                                                let new_len = elems.len() + 1;
                                                local.init = Some(Expr::Array(ExprArray {
                                                    attrs: array_expr.attrs.clone(),
                                                    bracket_token: array_expr.bracket_token,
                                                    elems: parse_quote!([(); #new_len]),
                                                }));
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
        "The mutation operator targets array declarations with a const expression as the size. It replaces the const expression with a different constant expression, potentially triggering bugs related to const evaluation and borrow checking."
    }
}