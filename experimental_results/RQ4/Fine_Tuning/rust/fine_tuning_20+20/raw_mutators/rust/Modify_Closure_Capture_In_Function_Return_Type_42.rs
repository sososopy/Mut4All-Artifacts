use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Modify_Closure_Capture_In_Function_Return_Type_42;

impl Mutator for Modify_Closure_Capture_In_Function_Return_Type_42 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_In_Function_Return_Type_42"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut has_fn_bound = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound
                                    .path
                                    .segments
                                    .last()
                                    .map_or(false, |seg| seg.ident == "Fn")
                                {
                                    has_fn_bound = true;
                                    break;
                                }
                            }
                        }
                        if has_fn_bound {
                            let mut found_closure = false;
                            let mut new_stmts = Vec::new();
                            for stmt in &item_fn.block.stmts {
                                if let Stmt::Expr(Expr::Closure(closure), _) = stmt {
                                    if !found_closure {
                                        found_closure = true;
                                        let mut new_capture = None;
                                        for capture in &closure.capture {
                                            if let Expr::Path(ExprPath { path, .. }) = capture {
                                                if let Some(ident) = path.get_ident() {
                                                    let new_ident =
                                                        Ident::new(&format!("{}_new", ident), ident.span());
                                                    new_capture = Some(new_ident.clone());
                                                    new_stmts.push(Stmt::Local(Local {
                                                        attrs: Vec::new(),
                                                        let_token: Default::default(),
                                                        pat: Pat::Type(PatType {
                                                            attrs: Vec::new(),
                                                            pat: Box::new(Pat::Ident(
                                                                syn::PatIdent {
                                                                    attrs: Vec::new(),
                                                                    by_ref: None,
                                                                    mutability: None,
                                                                    ident: new_ident,
                                                                    subpat: None,
                                                                },
                                                            )),
                                                            colon_token: Default::default(),
                                                            ty: Box::new(Type::Path(TypePath {
                                                                qself: None,
                                                                path: syn::Path::from(Ident::new(
                                                                    "Vec",
                                                                    Span::call_site(),
                                                                )),
                                                            })),
                                                        }),
                                                        init: Some((
                                                            Default::default(),
                                                            Box::new(Expr::Array(syn::ExprArray {
                                                                attrs: Vec::new(),
                                                                bracket_token: Default::default(),
                                                                elems: {
                                                                    let mut elems =
                                                                        Punctuated::new();
                                                                    elems.push(Expr::Lit(
                                                                        syn::ExprLit {
                                                                            attrs: Vec::new(),
                                                                            lit: syn::Lit::Int(
                                                                                syn::LitInt::new(
                                                                                    "1",
                                                                                    Span::call_site(
                                                                                    ),
                                                                                ),
                                                                            ),
                                                                        },
                                                                    ));
                                                                    elems.push(Expr::Lit(
                                                                        syn::ExprLit {
                                                                            attrs: Vec::new(),
                                                                            lit: syn::Lit::Int(
                                                                                syn::LitInt::new(
                                                                                    "2",
                                                                                    Span::call_site(
                                                                                    ),
                                                                                ),
                                                                            ),
                                                                        },
                                                                    ));
                                                                    elems.push(Expr::Lit(
                                                                        syn::ExprLit {
                                                                            attrs: Vec::new(),
                                                                            lit: syn::Lit::Int(
                                                                                syn::LitInt::new(
                                                                                    "3",
                                                                                    Span::call_site(
                                                                                    ),
                                                                                ),
                                                                            ),
                                                                        },
                                                                    ));
                                                                    elems
                                                                },
                                                            })),
                                                        )),
                                                        semi_token: Default::default(),
                                                    }));
                                                }
                                            }
                                        }
                                        let mut new_capture_list = Vec::new();
                                        for capture in &closure.capture {
                                            new_capture_list.push(capture.clone());
                                        }
                                        if let Some(new_capture) = new_capture {
                                            new_capture_list.push(Expr::Path(ExprPath {
                                                attrs: Vec::new(),
                                                qself: None,
                                                path: syn::Path::from(new_capture),
                                            }));
                                        }
                                        let new_body = if let Some(new_capture) = new_capture {
                                            let mut new_body_stmts = Vec::new();
                                            new_body_stmts.push(Stmt::Semi(
                                                Expr::MethodCall(syn::ExprMethodCall {
                                                    attrs: Vec::new(),
                                                    receiver: Box::new(Expr::Path(ExprPath {
                                                        attrs: Vec::new(),
                                                        qself: None,
                                                        path: syn::Path::from(new_capture),
                                                    })),
                                                    dot_token: Default::default(),
                                                    method: Ident::new("len", Span::call_site()),
                                                    turbofish: None,
                                                    paren_token: Default::default(),
                                                    args: Punctuated::new(),
                                                }),
                                                Default::default(),
                                            ));
                                            new_body_stmts.push(Stmt::Expr(*closure.body.clone(), None));
                                            Expr::Block(syn::ExprBlock {
                                                attrs: Vec::new(),
                                                label: None,
                                                block: syn::Block {
                                                    brace_token: Default::default(),
                                                    stmts: new_body_stmts,
                                                },
                                            })
                                        } else {
                                            *closure.body.clone()
                                        };
                                        new_stmts.push(Stmt::Expr(
                                            Expr::Closure(syn::ExprClosure {
                                                attrs: closure.attrs.clone(),
                                                lifetimes: closure.lifetimes.clone(),
                                                constness: closure.constness,
                                                movability: closure.movability,
                                                asyncness: closure.asyncness,
                                                capture: new_capture_list,
                                                or1_token: closure.or1_token,
                                                inputs: closure.inputs.clone(),
                                                or2_token: closure.or2_token,
                                                output: closure.output.clone(),
                                                body: Box::new(new_body),
                                            }),
                                            None,
                                        ));
                                    }
                                } else {
                                    new_stmts.push(stmt.clone());
                                }
                            }
                            item_fn.block.stmts = new_stmts;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions returning `impl Fn` by modifying the closure's captured variables. It introduces a new variable with a different type and ensures the closure captures and uses this new variable, increasing the complexity of closure capturing and potentially exposing compiler bugs related to closure environment handling."
    }
}