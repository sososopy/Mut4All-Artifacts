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

pub struct Modify_Associated_Type_Lifetime_411;

impl Mutator for Modify_Associated_Type_Lifetime_411 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_Lifetime_411"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut existing_lifetimes = HashSet::new();
                for lifetime in &item_impl.generics.params {
                    if let syn::GenericParam::Lifetime(lifetime_param) = lifetime {
                        existing_lifetimes.insert(lifetime_param.lifetime.ident.to_string());
                    }
                }
                
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(impl_type) = impl_item {
                        let lifetime_option = extract_lifetime(&impl_type.ty);
                        if let Some(lifetime) = lifetime_option {
                            if existing_lifetimes.contains(&lifetime.ident.to_string()) {
                                let new_lifetime = generate_conflicting_lifetime(&existing_lifetimes);
                                replace_lifetime(&mut impl_type.ty, &lifetime, &new_lifetime);
                            }
                        }
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets associated types in `impl` blocks, modifying their lifetime parameters to create conflicts with existing lifetimes. This is intended to test the compiler's ability to resolve conflicting lifetime specifications, potentially leading to ICEs or other errors related to lifetime resolution."
    }
}

fn extract_lifetime(ty: &Type) -> Option<&Lifetime> {
    if let Type::Path(TypePath { path, .. }) = ty {
        for segment in &path.segments {
            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                for arg in &args.args {
                    if let GenericArgument::Lifetime(lifetime) = arg {
                        return Some(lifetime);
                    }
                }
            }
        }
    }
    None
}

fn generate_conflicting_lifetime(existing_lifetimes: &HashSet<String>) -> Lifetime {
    let mut rng = thread_rng();
    loop {
        let new_lifetime = format!("'{}", rng.gen_range('a'..='z'));
        if !existing_lifetimes.contains(&new_lifetime) {
            return Lifetime::new(&new_lifetime, Span::call_site());
        }
    }
}

fn replace_lifetime(ty: &mut Type, old: &Lifetime, new: &Lifetime) {
    if let Type::Path(TypePath { path, .. }) = ty {
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Lifetime(lifetime) = arg {
                        if lifetime == old {
                            *lifetime = new.clone();
                        }
                    }
                }
            }
        }
    }
}