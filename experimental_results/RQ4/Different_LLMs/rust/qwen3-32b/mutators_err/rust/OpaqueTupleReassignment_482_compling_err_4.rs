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

pub struct OpaqueTupleReassignment_482;

impl Mutator for OpaqueTupleReassignment_482 {
    fn name(&self) -> &str {
        "OpaqueTupleReassignment_482"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if !func.sig.generics.params.is_empty() {
                    if let ReturnType::Type(_, return_type) = &func.sig.output {
                        if let Type::ImplTrait(_) = &**return_type {
                            let mut found = false;
                            for i in 0..func.block.stmts.len() {
                                if let Stmt::Local(local) = &func.block.stmts[i] {
                                    if let Pat::Type(pat_type) = &local.pat {
                                        if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                            if let Some(init) = &local.init {
                                                if let Some(segment) = pat_type.ty.path.get_ident() {
                                                    if segment == "T" {
                                                        let new_code = quote! {
                                                            type Opaque<T> = impl Sized;
                                                            let y: Opaque<T> = (#pat_ident,);
                                                            let (z,): (T,) = y;
                                                        };
                                                        let new_block: syn::Block = syn::parse2(new_code).unwrap();
                                                        func.block.stmts.splice(i..i+1, new_block.stmts);
                                                        found = true;
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if !found {
                                let new_var = parse_quote! {
                                    let x: T = unimplemented!();
                                };
                                func.block.stmts.insert(0, new_var);
                                let new_code = quote! {
                                    type Opaque<T> = impl Sized;
                                    let y: Opaque<T> = (x,);
                                    let (z,): (T,) = y;
                                };
                                let new_block: syn::Block = syn::parse2(new_code).unwrap();
                                func.block.stmts.splice(0..1, new_block.stmts);
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