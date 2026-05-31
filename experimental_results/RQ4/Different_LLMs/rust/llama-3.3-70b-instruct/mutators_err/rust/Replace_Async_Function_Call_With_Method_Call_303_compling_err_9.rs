use syn::{parse_quote, Expr, ExprCall, ExprPath, Ident, Stmt, TraitItem, ItemTrait, Token, trait_item::Method};
use crate::mutator::Mutator;

struct Replace_Async_Function_Call_With_Method_Call_303;

impl Mutator for Replace_Async_Function_Call_With_Method_Call_303 {
    fn name(&self) -> &str {
        "Replace_Async_Function_Call_With_Method_Call_303"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for method in &mut trait_item.items {
                    if let TraitItem::Method(method) = method {
                        for statement in &mut method.block.stmts {
                            if let Stmt::Expr(expr) = statement {
                                if let Expr::Call(ExprCall { func, args, .. }) = expr {
                                    if let Expr::Path(ExprPath { path, .. }) = func {
                                        if path.segments.last().unwrap().ident.to_string().starts_with("async_") {
                                            let method_name = path.segments.last().unwrap().ident.clone();
                                            let method_name_str = method_name.to_string();
                                            let new_method_name = method_name_str.trim_start_matches("async_");
                                            let new_method_ident = Ident::new(&new_method_name, method_name.span());
                                            let new_expr = parse_quote! {
                                                self.#new_method_ident(#args)
                                            };
                                            *expr = new_expr;
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
        "The mutation operator targets async function calls within traits and replaces them with method calls. It aims to test the compiler's handling of async functions in traits by introducing method calls that may alter the execution flow or type checking."
    }
}