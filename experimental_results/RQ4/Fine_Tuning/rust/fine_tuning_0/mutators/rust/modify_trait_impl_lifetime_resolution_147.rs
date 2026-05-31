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

pub struct Modify_Trait_Impl_Lifetime_Resolution_147;

impl Mutator for Modify_Trait_Impl_Lifetime_Resolution_147 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_Lifetime_Resolution_147"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            let mut new_bounds = Punctuated::new();
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let new_trait_bound = TraitBound {
                                        lifetimes: Some(parse_quote!(for<'b>)),
                                        path: trait_bound.path.clone(),
                                        modifier: trait_bound.modifier.clone(),
                                        paren_token: trait_bound.paren_token.clone(),
                                    };
                                    new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                            type_impl_trait.bounds = new_bounds;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function signatures using `impl Trait` with lifetimes and modifies them to introduce conflicting or unresolved lifetimes. By altering the lifetime resolution in trait bounds, the mutation aims to provoke lifetime resolution issues or ICEs in the Rust compiler."
    }
}