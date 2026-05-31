use syn::{parse_quote, Expr, ExprCall, Stmt};
use crate::mutator::Mutator;

struct ReplaceConstFnCallWithNonConstFnCall_395;

impl Mutator for ReplaceConstFnCallWithNonConstFnCall_395 {
    fn name(&self) -> &str {
        "ReplaceConstFnCallWithNonConstFnCall_395"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Call(ExprCall {
                        attrs: _,
                        func: Expr::Path(path),
                        paren_token: _,
                        args: _,
                    }), _) = stmt
                    {
                        let new_stmt = Stmt::Expr(
                            parse_quote! {
                                {
                                    let fun = #path;
                                    #path(#func.sig.inputs.iter().map(|arg| {
                                        match arg {
                                            syn::FnArg::Typed(PatType { pat, .. }) => {
                                                quote!(#pat)
                                            }
                                            _ => panic!("Unsupported argument type"),
                                        }
                                    }).collect::<Vec<_>>());
                                }
                            },
                            None,
                        );
                        new_stmts.push(new_stmt);
                    } else {
                        new_stmts.push(stmt.clone());
                    }
                }
                func.block.stmts = new_stmts;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant function calls in the code and replaces them with non-constant function calls. This transformation aims to trigger bugs related to the evaluation of constant expressions and the interaction between constant and non-constant code."
    }
}