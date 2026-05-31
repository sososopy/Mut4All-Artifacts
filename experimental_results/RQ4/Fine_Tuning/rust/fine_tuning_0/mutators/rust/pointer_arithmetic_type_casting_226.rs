use syn::{Expr, ExprCast, Stmt, parse::Parse, parse_quote, Local, Pat, LocalInit};
use crate::mutator::Mutator;

pub struct Pointer_Arithmetic_Type_Casting_226;

impl Mutator for Pointer_Arithmetic_Type_Casting_226 {
    fn name(&self) -> &str {
        "Pointer_Arithmetic_Type_Casting_226"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let Expr::MethodCall(method_call) = &*init.expr {
                                if method_call.method == "add" {
                                    if let Expr::Cast(ExprCast { expr: ptr_expr, ty, .. }) = &*method_call.receiver {
                                        if let Expr::Lit(expr_lit) = &method_call.args[0] {
                                            let ptr_as_usize: Expr = parse_quote! { #ptr_expr as usize };
                                            let new_address: Expr = parse_quote! { #ptr_as_usize + #expr_lit * std::mem::size_of::<#ty>() };
                                            let new_ptr: Expr = parse_quote! { #new_address as *const #ty };
                                            let pat: Pat = local.pat.clone();
                                            *stmt = Stmt::Local(Local {
                                                pat,
                                                init: Some(LocalInit {
                                                    eq_token: syn::token::Eq::default(),
                                                    expr: Box::new(new_ptr),
                                                    diverge: None,
                                                }),
                                                ..local.clone()
                                            });
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
        "The mutation operator targets pointer arithmetic within unsafe blocks. It replaces direct pointer arithmetic with a sequence of operations involving casting the pointer to a usize, performing arithmetic, and casting back to the original pointer type. This transformation aims to test the compiler's handling of pointer arithmetic and type casting, potentially revealing issues related to memory safety and type inference."
    }
}