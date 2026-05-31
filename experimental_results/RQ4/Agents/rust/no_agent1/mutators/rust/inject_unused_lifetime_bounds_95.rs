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

pub struct Inject_Unused_Lifetime_Bounds_95;

impl Mutator for Inject_Unused_Lifetime_Bounds_95 {
    fn name(&self) -> &str {
        "Inject_Unused_Lifetime_Bounds_95"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_bound = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'unused", Span::call_site()),
                    colon_token: Some(token::Colon::default()),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(Lifetime::new("'static", Span::call_site()));
                        bounds
                    },
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_bound));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetime_bound = LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'unused", Span::call_site()),
                            colon_token: Some(token::Colon::default()),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(Lifetime::new("'static", Span::call_site()));
                                bounds
                            },
                        };
                        func.sig.generics.params.push(GenericParam::Lifetime(lifetime_bound));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator injects unused lifetime bounds into function and method signatures. By introducing lifetimes that are not utilized within the function body, it aims to stress-test the compiler's lifetime resolution system, potentially triggering ICEs or assertion failures related to lifetime inference and unused bounds."
    }
}