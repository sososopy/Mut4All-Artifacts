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

pub struct Type_Parameter_Name_Mismatch_31;

impl Mutator for Type_Parameter_Name_Mismatch_31 {
    fn name(&self) -> &str {
        "Type_Parameter_Name_Mismatch_31"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                let generics = &mut item_fn.sig.generics.params;
                let mut rng = thread_rng();

                if !generics.is_empty() {
                    let mut new_generics = generics.clone();
                    for param in &mut new_generics {
                        if let GenericParam::Type(type_param) = param {
                            let new_ident = Ident::new(
                                &format!("{}_Mut", type_param.ident),
                                type_param.ident.span(),
                            );
                            type_param.ident = new_ident;
                        }
                    }

                    *generics = new_generics.clone();

                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some((_, expr)) = &local.init {
                                if let Expr::Path(expr_path) = &**expr {
                                    if let Some(segment) = expr_path.path.segments.last() {
                                        if let Some(GenericParam::Type(type_param)) = new_generics.iter().find(|param| {
                                            if let GenericParam::Type(type_param) = param {
                                                type_param.ident == segment.ident
                                            } else {
                                                false
                                            }
                                        }) {
                                            let new_ident = Ident::new(
                                                &format!("{}_Mut", type_param.ident),
                                                type_param.ident.span(),
                                            );
                                            let mut new_path = expr_path.clone();
                                            new_path.path.segments.last_mut().unwrap().ident = new_ident;
                                            *stmt = Stmt::Local(syn::Local {
                                                init: Some((Default::default(), Box::new(Expr::Path(new_path)))),
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
        }
    }

    fn chain_of_thought(&self) -> &str {
        ""
    }
}