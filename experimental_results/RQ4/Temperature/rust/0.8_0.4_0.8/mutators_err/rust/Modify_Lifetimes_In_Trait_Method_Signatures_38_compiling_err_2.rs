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
    ItemTrait, TraitItem, TraitItemMethod,
};

use crate::mutator::Mutator;

pub struct Modify_Lifetimes_In_Trait_Method_Signatures_38;

impl Mutator for Modify_Lifetimes_In_Trait_Method_Signatures_38 {
    fn name(&self) -> &str {
        "Modify_Lifetimes_In_Trait_Method_Signatures_38"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut used_lifetimes = HashSet::new();
                for generics in &trait_item.generics.params {
                    if let syn::GenericParam::Lifetime(lifetime) = generics {
                        used_lifetimes.insert(lifetime.lifetime.ident.clone());
                    }
                }
                
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = item {
                        if let syn::ReturnType::Type(_, ty) = &mut method.sig.output {
                            if let syn::Type::Path(type_path) = &mut **ty {
                                for segment in &mut type_path.path.segments {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Lifetime(lifetime) = arg {
                                                let new_lifetime = Ident::new("b", Span::call_site());
                                                if !used_lifetimes.contains(&new_lifetime) {
                                                    *lifetime = Lifetime::new("'b", Span::call_site());
                                                    used_lifetimes.insert(new_lifetime);
                                                } else {
                                                    let new_lifetime = Ident::new("c", Span::call_site());
                                                    *lifetime = Lifetime::new("'c", Span::call_site());
                                                    used_lifetimes.insert(new_lifetime);
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}