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

pub struct Inject_Invalid_Lifetime_Bounds_287;

impl Mutator for Inject_Invalid_Lifetime_Bounds_287 {
    fn name(&self) -> &str {
        "Inject_Invalid_Lifetime_Bounds_287"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let invalid_lifetime = Lifetime::new("'invalid", Span::call_site());
                let mut bounds = Punctuated::new();
                bounds.push(TypeParamBound::Lifetime(invalid_lifetime.clone()));
                func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: vec![],
                    lifetime: invalid_lifetime.clone(),
                    colon_token: None,
                    bounds,
                }));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let invalid_lifetime = Lifetime::new("'invalid", Span::call_site());
                        let mut bounds = Punctuated::new();
                        bounds.push(TypeParamBound::Lifetime(invalid_lifetime.clone()));
                        func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: vec![],
                            lifetime: invalid_lifetime.clone(),
                            colon_token: None,
                            bounds,
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a lifetime parameter with an invalid bound into function and method signatures. By adding a lifetime that is not properly bound, it aims to provoke lifetime resolution errors, potentially leading to ICEs or assertion failures in the compiler's borrow checker and lifetime inference system."
    }
}