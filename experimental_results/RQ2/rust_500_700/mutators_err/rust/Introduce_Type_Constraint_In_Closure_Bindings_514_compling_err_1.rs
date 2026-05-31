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

pub struct Introduce_Type_Constraint_In_Closure_Bindings_514;

impl Mutator for Introduce_Type_Constraint_In_Closure_Bindings_514 {
    fn name(&self) -> &str {
        "Introduce_Type_Constraint_In_Closure_Bindings_514"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let syn::Stmt::Local(local) = stmt {
                        if let syn::Pat::Ident(pat_ident) = &local.pat {
                            if let Some((_, expr)) = &local.init {
                                if let syn::Expr::Closure(closure) = &**expr {
                                    let mut new_inputs = Punctuated::new();
                                    if closure.inputs.is_empty() {
                                        new_inputs.push(FnArg::Typed(PatType {
                                            attrs: Vec::new(),
                                            pat: Box::new(Pat::Wild(PatWild {
                                                underscore_token: token::Underscore::default(),
                                            })),
                                            colon_token: token::Colon::default(),
                                            ty: Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath::from(Ident::new("i32", Span::call_site())),
                                            })),
                                        }));
                                    } else {
                                        for input in &closure.inputs {
                                            if let FnArg::Typed(pat_type) = input {
                                                let mut new_pat_type = pat_type.clone();
                                                if let Pat::Ident(_) = *new_pat_type.pat {
                                                    new_pat_type.ty = Box::new(Type::Path(TypePath {
                                                        qself: None,
                                                        path: SynPath::from(Ident::new("i32", Span::call_site())),
                                                    }));
                                                }
                                                new_inputs.push(FnArg::Typed(new_pat_type));
                                            }
                                        }
                                    }
                                    let new_closure = ExprClosure {
                                        inputs: new_inputs,
                                        ..closure.clone()
                                    };
                                    *stmt = syn::Stmt::Local(syn::Local {
                                        init: Some((local.init.as_ref().unwrap().0.clone(), Box::new(Expr::Closure(new_closure)))),
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
    fn chain_of_thought(&self) -> &str {
        ""
    }
}