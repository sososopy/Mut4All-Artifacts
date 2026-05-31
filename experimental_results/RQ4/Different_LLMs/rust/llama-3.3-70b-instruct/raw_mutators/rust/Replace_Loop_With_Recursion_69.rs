use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Loop_With_Recursion_69;

impl Mutator for Replace_Loop_With_Recursion_69 {
    fn name(&self) -> &str {
        "Replace_Loop_With_Recursion_69"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_block = Block {
                    brace_token: func.block.brace_token,
                    stmts: Punctuated::new(),
                };

                for stmt in &func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Loop(loop_expr) = &**expr {
                            let loop_block = loop_expr.block.clone();
                            let recursive_function_name = Ident::new(
                                &format!("recursive_{}", func.sig.ident),
                                Span::call_site(),
                            );

                            let recursive_function = ItemFn {
                                attrs: vec![],
                                vis: Visibility::Inherited,
                                sig: Signature {
                                    constness: None,
                                    asyncness: None,
                                    unsafety: None,
                                    abi: None,
                                    ident: recursive_function_name,
                                    generics: Generics {
                                        lifetimes: LifetimeParams::none(),
                                        type_params: TypeParamBounds::none(),
                                        where_clause: None,
                                    },
                                    inputs: func.sig.inputs.clone(),
                                    output: func.sig.output.clone(),
                                    variadic: false,
                                },
                                block: loop_block,
                            };

                            let recursive_call = Expr::Call(ExprCall {
                                attrs: vec![],
                                expr: Expr::Path(ExprPath {
                                    attrs: vec![],
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: recursive_function_name,
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                    qself: None,
                                }),
                                paren_token: Paren {
                                    span: Span::call_site(),
                                },
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

                func.block = new_block;

                file.items.push(Item::Fn(recursive_function));
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces loops with recursive function calls, which can help trigger bugs related to const evaluation and type checking. It aims to test the compiler's ability to handle recursive functions and const evaluation."
    }
}