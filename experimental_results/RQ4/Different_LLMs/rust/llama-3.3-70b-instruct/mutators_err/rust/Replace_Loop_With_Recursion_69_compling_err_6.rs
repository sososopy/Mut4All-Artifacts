use syn::{Block, Expr, ExprCall, ExprPath, Generics, Ident, Item, ItemFn, PathArguments, PathSegment, punctuated::Punctuated, Signature, Stmt, Token, Visibility};
use proc_macro2::Span;
use syn::token::{Lt, Gt, Paren};

struct Replace_Loop_With_Recursion_69;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Replace_Loop_With_Recursion_69 {
    fn name(&self) -> &str {
        "Replace_Loop_With_Recursion_69"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut new_block = Block {
                    brace_token: func.block.brace_token,
                    stmts: Vec::new(),
                };

                let mut recursive_function = None;

                for stmt in &func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Loop(loop_expr) = expr {
                            let loop_block = loop_expr.body.clone();
                            let recursive_function_name = Ident::new(
                                &format!("recursive_{}", func.sig.ident),
                                Span::call_site(),
                            );

                            let recursive_function_item = ItemFn {
                                attrs: vec![],
                                vis: Visibility::Inherited,
                                sig: Signature {
                                    constness: None,
                                    asyncness: None,
                                    unsafety: None,
                                    abi: None,
                                    fn_token: syn::token::Fn {
                                        span: Span::call_site(),
                                    },
                                    ident: recursive_function_name.clone(),
                                    generics: Generics {
                                        lt_token: Some(Lt { span: Span::call_site() }),
                                        params: Punctuated::new(),
                                        gt_token: Some(Gt { span: Span::call_site() }),
                                        where_clause: None,
                                    },
                                    paren_token: Paren { span: Span::call_site() },
                                    inputs: func.sig.inputs.clone(),
                                    output: func.sig.output.clone(),
                                    variadic: None,
                                },
                                block: Box::new(loop_block),
                            };

                            recursive_function = Some(recursive_function_item);

                            let recursive_call = Expr::Call(ExprCall {
                                attrs: vec![],
                                func: Box::new(Expr::Path(ExprPath {
                                    attrs: vec![],
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: recursive_function_name,
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                    qself: None,
                                })),
                                paren_token: Paren { span: Span::call_site() },
                                args: Punctuated::new(),
                            });

                            new_block.stmts.push(Stmt::Expr(recursive_call, None));
                        } else {
                            new_block.stmts.push(stmt.clone());
                        }
                    } else {
                        new_block.stmts.push(stmt.clone());
                    }
                }

                func.block = Box::new(new_block);

                if let Some(recursive_function_item) = recursive_function {
                    file.items.push(Item::Fn(recursive_function_item));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces loops with recursive function calls, which can help trigger bugs related to const evaluation and type checking. It aims to test the compiler's ability to handle recursive functions and const evaluation."
    }
}