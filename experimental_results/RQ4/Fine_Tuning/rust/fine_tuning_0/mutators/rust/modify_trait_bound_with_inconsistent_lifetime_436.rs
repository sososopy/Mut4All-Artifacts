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

pub struct Modify_Trait_Bound_With_Inconsistent_Lifetime_436;

impl Mutator for Modify_Trait_Bound_With_Inconsistent_Lifetime_436 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_With_Inconsistent_Lifetime_436"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            for bound in &mut type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(lifetime) = &trait_bound.lifetimes {
                                        let new_lifetime = Lifetime::new("'b", Span::call_site());
                                        let mut new_lifetimes = BoundLifetimes {
                                            for_token: lifetime.for_token,
                                            lt_token: lifetime.lt_token,
                                            lifetimes: Punctuated::new(),
                                            gt_token: lifetime.gt_token,
                                        };
                                        new_lifetimes.lifetimes.push(syn::GenericParam::Lifetime(LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: new_lifetime.clone(),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                        }));
                                        trait_bound.lifetimes = Some(new_lifetimes);
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