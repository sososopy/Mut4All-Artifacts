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

pub struct UnboxedClosureArgTupleViolation_495;

impl Mutator for UnboxedClosureArgTupleViolation_495 {
    fn name(&self) -> &str {
        "UnboxedClosureArgTupleViolation_495"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.abi.name() == "rust-call" && func.sig.inputs.len() == 1 {
                    if let FnArg::Typed(pat_type) = &mut func.sig.inputs[0] {
                        if let Pat::Tuple(tuple_pat) = &*pat_type.pat {
                            if let Type::Tuple(tuple_type) = &*pat_type.ty {
                                let new_args: Vec<_> = tuple_pat.elems.iter().zip(tuple_type.elems.iter()).map(|(pat, ty)| {
                                    FnArg::Typed(PatType {
                                        pat: Box::new(Pat::clone(pat)),
                                        ty: Box::new(Type::clone(ty)),
                                        colon_token: token::Colon::default(),
                                    })
                                }).collect();
                                func.sig.inputs.clear();
                                for arg in new_args {
                                    func.sig.inputs.push(arg);
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