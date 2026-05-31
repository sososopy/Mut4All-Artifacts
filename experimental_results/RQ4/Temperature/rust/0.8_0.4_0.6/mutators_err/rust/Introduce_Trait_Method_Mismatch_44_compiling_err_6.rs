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
    ItemTrait, TraitItem, ImplItem, Signature,
};

use crate::mutator::Mutator;

pub struct Introduce_Trait_Method_Mismatch_44;

impl Mutator for Introduce_Trait_Method_Mismatch_44 {
    fn name(&self) -> &str {
        "Introduce_Trait_Method_Mismatch_44"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_items = vec![];

        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                trait_items.push(trait_item);
            }
        }

        let mut items_to_mutate = vec![];

        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    items_to_mutate.push(item_impl);
                }
            }
        }

        for item_impl in items_to_mutate {
            for impl_item in &mut item_impl.items {
                if let syn::ImplItem::Fn(method) = impl_item {
                    let method_name = &method.sig.ident;
                    if let Some(trait_item) = trait_items.iter().find(|trait_item| {
                        trait_item.items.iter().any(|trait_method| {
                            if let syn::TraitItem::Fn(trait_method) = trait_method {
                                &trait_method.sig.ident == method_name
                            } else {
                                false
                            }
                        })
                    }) {
                        if let Some(syn::TraitItem::Fn(trait_method)) = trait_item
                            .items
                            .iter()
                            .find(|trait_item| {
                                if let syn::TraitItem::Fn(trait_method) = trait_item {
                                    &trait_method.sig.ident == method_name
                                } else {
                                    false
                                }
                            })
                        {
                            let mut rng = thread_rng();
                            let param_count = trait_method.sig.inputs.len();
                            if param_count > 0 && rng.gen_bool(0.5) {
                                method.sig.inputs.pop();
                            } else {
                                let new_param: FnArg = parse_quote!(param_extra: i32);
                                method.sig.inputs.push(new_param);
                            }
                        }
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait method implementations by introducing parameter mismatches. It either adds an extra parameter or removes an existing one, creating inconsistency with the trait definition. This transformation aims to expose compiler bugs related to trait method resolution and parameter handling."
    }
}