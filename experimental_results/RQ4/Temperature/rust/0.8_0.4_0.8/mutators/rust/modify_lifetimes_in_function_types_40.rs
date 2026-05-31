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

pub struct Modify_Lifetimes_In_Function_Types_40;

impl Mutator for Modify_Lifetimes_In_Function_Types_40 {
    fn name(&self) -> &str {
        "Modify_Lifetimes_In_Function_Types_40"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut lifetimes = HashSet::new();
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Lifetime(lifetime) = param {
                        lifetimes.insert(lifetime.lifetime.clone());
                    }
                }
                
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &mut *pat_type.ty {
                            if let Some(last_segment) = type_path.path.segments.last_mut() {
                                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                    for arg in &mut args.args {
                                        if let GenericArgument::Lifetime(lifetime) = arg {
                                            if lifetimes.contains(lifetime) {
                                                let new_lifetime = Lifetime::new("'b", Span::call_site());
                                                *lifetime = new_lifetime;
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies lifetimes in function types by replacing existing lifetimes with a new distinct lifetime `'b`. This transformation aims to test the compiler's handling of lifetime propagation and resolution in function types, potentially exposing issues related to lifetime bounds and elision."
    }
}