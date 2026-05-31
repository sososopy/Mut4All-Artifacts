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

pub struct Inject_Unused_Lifetimes_78;

impl Mutator for Inject_Unused_Lifetimes_78 {
    fn name(&self) -> &str {
        "Inject_Unused_Lifetimes_78"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let unused_lifetime = Lifetime::new("'unused", Span::call_site());
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: unused_lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(type_path) = &mut **ty {
                        type_path.path.segments.iter_mut().for_each(|segment| {
                            if let PathArguments::AngleBracketed(angle_bracketed) = &mut segment.arguments {
                                angle_bracketed.args.push(GenericArgument::Lifetime(unused_lifetime.clone()));
                            }
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter to function signatures and injects it into return type paths. This transformation aims to trigger lifetime resolution issues, potentially leading to ICEs or unexpected compiler behavior by introducing redundant and conflicting lifetime constraints."
    }
}