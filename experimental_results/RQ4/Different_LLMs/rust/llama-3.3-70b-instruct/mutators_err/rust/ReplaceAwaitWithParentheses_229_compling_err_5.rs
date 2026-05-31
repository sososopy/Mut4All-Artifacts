use syn::{parse_quote, Expr, ExprCall, ExprPath, Ident, Stmt};
use proc_macro2::Span;
use syn::punctuated::Punctuated;
use quote::quote;
use crate::mutator::Mutator;

struct ReplaceAwaitWithParentheses_229;

impl Mutator for ReplaceAwaitWithParentheses_229 {
    fn name(&self) -> &str {
        "ReplaceAwaitWithParentheses_229"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Await(await_expr) = expr {
                            let new_expr = parse_quote!(#await_expr.base);
                            *expr = Expr::Call(ExprCall {
                                attrs: await_expr.attrs.clone(),
                                func: Box::new(Expr::Path(ExprPath {
                                    attrs: await_expr.attrs.clone(),
                                    qself: None,
                                    path: syn::Path::from(Ident::new("await", Span::call_site())),
                                })),
                                args: Punctuated::from_iter([Expr::Verbatim(quote!(#new_expr))]),
                                paren_token: syn::token::Paren::default(),
                            });
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Await(await_expr) = expr {
                                    let new_expr = parse_quote!(#await_expr.base);
                                    *expr = Expr::Call(ExprCall {
                                        attrs: await_expr.attrs.clone(),
                                        func: Box::new(Expr::Path(ExprPath {
                                            attrs: await_expr.attrs.clone(),
                                            qself: None,
                                            path: syn::Path::from(Ident::new("await", Span::call_site())),
                                        })),
                                        args: Punctuated::from_iter([Expr::Verbatim(quote!(#new_expr))]),
                                        paren_token: syn::token::Paren::default(),
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces await expressions with await followed by an empty pair of parentheses. This transformation tests the compiler's ability to handle and parse await expressions correctly, potentially triggering bugs related to parsing and handling of await expressions."
    }
}