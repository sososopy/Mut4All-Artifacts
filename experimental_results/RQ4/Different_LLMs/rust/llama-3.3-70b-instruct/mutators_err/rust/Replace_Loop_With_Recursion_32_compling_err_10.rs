use syn::{Token, Stmt, Expr, Ident, ItemFn, Visibility, Signature, Generics, punctuated, ReturnType, ExprCall, ExprPath, PathSegment, PathArguments, Item};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Replace_Loop_With_Recursion_32;

impl Mutator for Replace_Loop_With_Recursion_32 {
    fn name(&self) -> &str {
        "Replace_Loop_With_Recursion_32"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Loop(loop_expr) = expr {
                            let loop_label = loop_expr.label.as_ref().map(|label| label.name.ident.clone());
                            let loop_body = loop_expr.body.clone();
                            let new_func_name = Ident::new(
                                &format!("{}_recursive", loop_label.as_ref().unwrap_or(&Ident::new("loop", Span::call_site()))),
                                Span::call_site(),
                            );
                            let new_func = ItemFn {
                                attrs: Vec::new(),
                                vis: Visibility::Inherited,
                                sig: Signature {
                                    fn_token: Token![fn](Span::call_site()),
                                    constness: None,
                                    asyncness: None,
                                    unsafety: None,
                                    abi: None,
                                    ident: new_func_name,
                                    generics: Generics::default(),
                                    paren_token: Default::default(),
                                    inputs: punctuated::Punctuated::new(),
                                    variadic: None,
                                    output: ReturnType::Default,
                                },
                                block: Box::new(loop_body),
                            };
                            new_stmts.push(Stmt::Expr(
                                Expr::Call(ExprCall {
                                    attrs: Vec::new(),
                                    func: Box::new(Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                                ident: new_func_name.clone(),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                        qself: None,
                                    })),
                                    paren_token: Default::default(),
                                    args: punctuated::Punctuated::new(),
                                }),
                                None,
                            ));
                            file.items.push(Item::Fn(new_func));
                        } else {
                            new_stmts.push(Stmt::Expr(expr.clone(), None));
                        }
                    } else {
                        new_stmts.push(stmt.clone());
                    }
                }
                func.block.stmts = new_stmts;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets loops in the given Rust code and replaces them with recursive functions. It applies to for loops and while loops. The operator will identify loops in the seed program, and then replace the loop with a recursive function call that achieves the same functionality."
    }
}