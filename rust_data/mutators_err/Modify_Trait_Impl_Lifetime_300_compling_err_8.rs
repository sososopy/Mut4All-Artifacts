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

pub struct Modify_Trait_Impl_Lifetime_300;

impl Mutator for Modify_Trait_Impl_Lifetime_300 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_Lifetime_300"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            for bound in &mut type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(lifetimes) = &trait_bound.lifetimes {
                                        let mut new_lifetimes = lifetimes.clone();
                                        for lifetime in &mut new_lifetimes.lifetimes {
                                            let new_lifetime_ident = Ident::new("b", lifetime.lifetime.span());
                                            lifetime.lifetime = Lifetime::new(&format!("'{}", new_lifetime_ident), Span::call_site());
                                        }
                                        trait_bound.lifetimes = Some(new_lifetimes);
                                    }
                                    for segment in &mut trait_bound.path.segments {
                                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Lifetime(lifetime) = arg {
                                                    let new_lifetime_ident = Ident::new("d", lifetime.ident.span());
                                                    *lifetime = Lifetime::new(&format!("'{}", new_lifetime_ident), Span::call_site());
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
        "The mutation operator targets function signatures using traits with complex lifetime and type parameters. It modifies the lifetimes in trait bounds to introduce unresolved or conflicting lifetimes, aiming to create scenarios where the Rust compiler faces lifetime resolution issues, potentially leading to an ICE."
    }
}