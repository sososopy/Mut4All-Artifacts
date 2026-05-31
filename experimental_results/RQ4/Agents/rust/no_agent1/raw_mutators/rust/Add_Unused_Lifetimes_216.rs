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

pub struct Add_Unused_Lifetimes_216;

impl Mutator for Add_Unused_Lifetimes_216 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetimes_216"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    let lifetime_names = vec!["'a", "'b", "'c", "'d"];
                    for lifetime_name in lifetime_names {
                        generics.params.push(syn::GenericParam::Lifetime(syn::LifetimeDef {
                            attrs: vec![],
                            lifetime: syn::Lifetime::new(lifetime_name, Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            let lifetime_names = vec!["'e", "'f", "'g", "'h"];
                            for lifetime_name in lifetime_names {
                                generics.params.push(syn::GenericParam::Lifetime(syn::LifetimeDef {
                                    attrs: vec![],
                                    lifetime: syn::Lifetime::new(lifetime_name, Span::call_site()),
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces multiple unused lifetimes into function and method signatures. By adding lifetimes that are not referenced within the function body or parameters, it aims to stress the compiler's lifetime resolution and checking mechanisms. This transformation can lead to unexpected behavior in lifetime inference and potentially trigger ICEs or other deep compiler bugs related to lifetime handling."
    }
}