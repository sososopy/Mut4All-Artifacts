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

pub struct Remove_Lifetime_From_Async_Trait_Impl_301;

impl Mutator for Remove_Lifetime_From_Async_Trait_Impl_301 {
    fn name(&self) -> &str {
        "Remove_Lifetime_From_Async_Trait_Impl_301"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_defs = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &trait_item.items {
                    if let TraitItem::Fn(method) = trait_item {
                        if method.sig.asyncness.is_some() && !method.sig.generics.params.is_empty() {
                            trait_defs.push((trait_item.span(), &method.sig));
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(impl_block) = item {
                for impl_item in &mut impl_block.items {
                    if let ImplItem::Fn(impl_method) = impl_item {
                        if impl_method.sig.asyncness.is_some() && !impl_method.sig.generics.params.is_empty() {
                            let lifetime_params: Vec<&LifetimeParam> = impl_method.sig.generics.params.iter()
                                .filter_map(|param| {
                                    if let GenericParam::Lifetime(lifetime) = param {
                                        Some(lifetime)
                                    } else {
                                        None
                                    }
                                })
                                .collect();
                            if !lifetime_params.is_empty() {
                                let mut rng = thread_rng();
                                let index_to_remove = rng.gen_range(0..lifetime_params.len());
                                let mut new_params = Punctuated::new();
                                for (i, param) in impl_method.sig.generics.params.iter().enumerate() {
                                    if let GenericParam::Lifetime(lifetime) = param {
                                        if i != index_to_remove {
                                            new_params.push(GenericParam::Lifetime(LifetimeParam {
                                                attrs: lifetime.attrs.clone(),
                                                lifetime: lifetime.lifetime.clone(),
                                                colon_token: lifetime.colon_token,
                                                bounds: lifetime.bounds.clone(),
                                            }));
                                        }
                                    } else {
                                        new_params.push(param.clone());
                                    }
                                }
                                impl_method.sig.generics.params = new_params;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async trait method implementations and removes one lifetime parameter from their generic parameter list. This creates a mismatch between the trait definition (which retains the lifetime) and the implementation, potentially triggering internal compiler errors during region substitution or type checking for async trait methods. The mutation preserves the method body and other signature elements to focus on the lifetime parameter discrepancy."
    }
}