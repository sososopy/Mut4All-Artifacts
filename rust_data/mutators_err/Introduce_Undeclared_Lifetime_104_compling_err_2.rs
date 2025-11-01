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

pub struct Introduce_Undeclared_Lifetime_104;

impl Mutator for Introduce_Undeclared_Lifetime_104 {
    fn name(&self) -> &str {
        "Introduce_Undeclared_Lifetime_104"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    if let Type::Path(type_path) = &*item_impl.self_ty {
                        if let Some(last_segment) = type_path.path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                let declared_lifetimes: HashSet<_> = item_impl.generics.lifetimes()
                                    .map(|lt| lt.lifetime.ident.to_string())
                                    .collect();

                                for arg in &mut args.args {
                                    if let GenericArgument::Lifetime(lifetime) = arg {
                                        if declared_lifetimes.contains(&lifetime.ident.to_string()) {
                                            let new_lifetime = Lifetime::new("'c", Span::call_site());
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementation blocks. It introduces an undeclared lifetime parameter by replacing a declared lifetime in the struct with a new undeclared one, creating a mismatch. This can lead to lifetime resolution errors."
    }
}