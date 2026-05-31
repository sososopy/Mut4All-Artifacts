use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Alter_Closure_Structure_106;

impl Mutator for Alter_Closure_Structure_106 {
    fn name(&self) -> &str {
        "Alter_Closure_Structure_106"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureFinder {
            found: Vec<ExprClosure>,
        }
        impl<'ast> Visit<'ast> for ClosureFinder {
            fn visit_expr_closure(&mut self, i: &'ast ExprClosure) {
                self.found.push(i.clone());
            }
        }
        let mut finder = ClosureFinder { found: vec![] };
        finder.visit_file(file);
        for closure in finder.found {
            if let Expr::Block(block) = &*closure.body {
                if block.block.stmts.iter().any(|stmt| match stmt {
                    Stmt::Item(Item::Verbatim(_)) => true,
                    _ => false,
                }) {
                    let mut new_stmts = vec![];
                    new_stmts.push(Stmt::Local(Local {
                        attrs: vec![],
                        let_token: token::Let {
                            span: Span::call_site(),
                        },
                        pat: Pat::Ident(syn::PatIdent {
                            attrs: vec![],
                            by_ref: None,
                            mutability: None,
                            ident: Ident::new("c", Span::call_site()),
                            subpat: None,
                        }),
                        init: Some(LocalInit {
                            eq_token: token::Eq {
                                spans: [Span::call_site()],
                            },
                            expr: Box::new(Expr::Block(ExprBlock {
                                attrs: vec![],
                                label: None,
                                block: Block {
                                    brace_token: token::Brace {
                                        span: Span::call_site(),
                                    },
                                    stmts: vec![Stmt::Expr(
                                        Expr::Closure(ExprClosure {
                                            attrs: closure.attrs.clone(),
                                            lifetimes: closure.lifetimes.clone(),
                                            constness: closure.constness,
                                            movability: closure.movability,
                                            asyncness: closure.asyncness,
                                            capture: closure.capture,
                                            or1_token: closure.or1_token,
                                            inputs: closure.inputs.clone(),
                                            or2_token: closure.or2_token,
                                            output: closure.output.clone(),
                                            body: Box::new(Expr::Unsafe(ExprUnsafe {
                                                attrs: vec![],
                                                unsafe_token: token::Unsafe {
                                                    span: Span::call_site(),
                                                },
                                                block: Block {
                                                    brace_token: token::Brace {
                                                        span: Span::call_site(),
                                                    },
                                                    stmts: vec![],
                                                },
                                            })),
                                        }),
                                        None,
                                    )],
                                },
                            })),
                            diverge: None,
                        }),
                        semi_token: token::Semi {
                            spans: [Span::call_site()],
                        },
                    }));
                    for stmt in &block.block.stmts {
                        if let Stmt::Item(Item::Verbatim(item_unsafe)) = stmt {
                            new_stmts.push(Stmt::Expr(
                                Expr::Block(ExprBlock {
                                    attrs: vec![],
                                    label: None,
                                    block: Block {
                                        brace_token: token::Brace {
                                            span: Span::call_site(),
                                        },
                                        stmts: vec![Stmt::Item(Item::Verbatim(
                                            item_unsafe.clone(),
                                        ))],
                                    },
                                }),
                                None,
                            ));
                        }
                    }
                    block.block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets closures containing unsafe blocks. It restructures the closure by moving the unsafe block outside the closure's body, wrapping the closure in a block, and misplacing closure delimiters. This transformation exploits potential weaknesses in the compiler's handling of closure syntax, unsafe code, and parameter binding, aiming to trigger parsing or semantic analysis errors."
    }
}