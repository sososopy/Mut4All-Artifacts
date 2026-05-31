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

pub struct Swap_Impl_Trait_Returns_14;

impl Mutator for Swap_Impl_Trait_Returns_14 {
    fn name(&self) -> &str {
        "Swap_Impl_Trait_Returns_14"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_map: HashMap<String, Type> = HashMap::new();
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if let Some(cfg_attr) = item_fn.attrs.iter().find(|attr| attr.path.is_ident("cfg")) {
                            let cfg_meta = cfg_attr.parse_meta().unwrap();
                            if let Meta::List(meta_list) = cfg_meta {
                                if let Some(NestedMeta::Meta(Meta::Path(path))) = meta_list.nested.first() {
                                    let cfg_name = path.segments.last().unwrap().ident.to_string();
                                    trait_map.insert(cfg_name, (**return_type).clone());
                                }
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        if let Some(cfg_attr) = item_fn.attrs.iter().find(|attr| attr.path.is_ident("cfg")) {
                            let cfg_meta = cfg_attr.parse_meta().unwrap();
                            if let Meta::List(meta_list) = cfg_meta {
                                if let Some(NestedMeta::Meta(Meta::Path(path))) = meta_list.nested.first() {
                                    let cfg_name = path.segments.last().unwrap().ident.to_string();
                                    if let Some(new_type) = trait_map.get(&cfg_name) {
                                        *return_type = Box::new(new_type.clone());
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
        "This mutator identifies functions with `impl Trait` return types under conditional compilation attributes and swaps the return traits between different configurations. It aims to test the compiler's handling of trait resolution and conditional compilation by altering the expected return traits in different build configurations."
    }
}