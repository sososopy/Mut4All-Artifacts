use syn::{parse_quote, Expr, Stmt};
use rand::Rng;
use rand::thread_rng;
use crate::mutator::Mutator;

struct Replace_Conditional_Expression_With_Constant_428;

impl Mutator for Replace_Conditional_Expression_With_Constant_428 {
    fn name(&self) -> &str {
        "Replace_Conditional_Expression_With_Constant_428"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::If(if_expr) = expr {
                            let mut rng = thread_rng();
                            let replace_with = if rng.gen_bool(0.5) {
                                parse_quote! { true }
                            } else {
                                parse_quote! { false }
                            };
                            *expr = replace_with;
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::If(if_expr) = expr {
                                    let mut rng = thread_rng();
                                    let replace_with = if rng.gen_bool(0.5) {
                                        parse_quote! { true }
                                    } else {
                                        parse_quote! { false }
                                    };
                                    *expr = replace_with;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces conditional expressions with constant boolean values (true or false). This transformation tests the compiler's handling of constant folding and conditional expressions, potentially exposing bugs related to constant folding, conditional expressions, and compiler optimizations."
    }
}