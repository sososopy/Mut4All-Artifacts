use syn::{parse_quote, Expr, ExprCall, ExprPath, Ident, PathArguments, PathSegment, Punctuated, Stmt};
use proc_macro2::Span;
use syn::visit_mut::VisitMut;
use syn::token::Paren;

struct Replace_Await_Expression_17;

impl Mutator for Replace_Await_Expression_17 {
    fn name(&self) -> &str {
        "Replace_Await_Expression_17"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Await(await_expr) = expr {
                                let block_on_path = syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("block_on", Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                };
                                let block_on_expr = Expr::Path(ExprPath {
                                    qself: None,
                                    attrs: vec![],
                                    path: block_on_path,
                                });
                                let block_on_call = Expr::Call(ExprCall {
                                    attrs: vec![],
                                    func: Box::new(block_on_expr),
                                    paren_token: Paren {
                                        span: Span::call_site(),
                                    },
                                    args: {
                                        let mut args = Punctuated::new();
                                        args.push(*await_expr.base.clone());
                                        args
                                    },
                                });
                                *expr = block_on_call;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions and replaces await expressions with a blocking call to the awaited future using `block_on`. This transformation removes the asynchronous behavior and tests the compiler's handling of async/await functionality."
    }
}

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}