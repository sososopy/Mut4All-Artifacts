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

pub struct Alter_Trait_Method_Lifetime_Bound_To_Tautological_250;

impl Mutator for Alter_Trait_Method_Lifetime_Bound_To_Tautological_250 {
    fn name(&self) -> &str {
        "Alter_Trait_Method_Lifetime_Bound_To_Tautological_250"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        for param in &mut method.sig.generics.params {
                            if let syn::GenericParam::Lifetime(lifetime_param) = param {
                                if let Some(bound) = &mut lifetime_param.bounds {
                                    if bound.len() == 1 {
                                        let bound_lifetime = bound.first().unwrap();
                                        if let syn::GenericParam::Lifetime(bound_lifetime_param) = bound_lifetime {
                                            if bound_lifetime_param.lifetime.ident != lifetime_param.lifetime.ident {
                                                bound.clear();
                                                bound.push(syn::GenericParam::Lifetime(syn::LifetimeParam {
                                                    attrs: lifetime_param.attrs.clone(),
                                                    lifetime: lifetime_param.lifetime.clone(),
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait method signatures with lifetime bounds. It changes any bound lifetime to a tautological self-reference (e.g., 'a: 'b becomes 'a: 'a). This transformation creates trivially satisfied bounds that may expose inconsistencies in the compiler's handling of lifetime bounds, especially in combination with complex features like impl Trait or Fn family traits."
    }
}