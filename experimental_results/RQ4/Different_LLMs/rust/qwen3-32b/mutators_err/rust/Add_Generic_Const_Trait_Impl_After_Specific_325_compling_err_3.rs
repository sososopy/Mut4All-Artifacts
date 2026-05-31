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

pub struct Add_Generic_Const_Trait_Impl_After_Specific_325;

impl Mutator for Add_Generic_Const_Trait_Impl_After_Specific_325 {
    fn name(&self) -> &str {
        "Add_Generic_Const_Trait_Impl_After_Specific_325"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_traits = HashSet::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                let has_attr = trait_item.attrs.iter().any(|attr| {
                    attr.path().segments.iter().any(|seg| seg.ident == "const_trait")
                });
                if has_attr {
                    let trait_ident = &trait_item.ident;
                    const_traits.insert(trait_ident.to_string());
                }
            }
        }

        let mut processed_traits = HashSet::new();

        for i in 0..file.items.len() {
            let item = &mut file.items[i];
            if let Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    let trait_ident = trait_path.segments.last().unwrap().ident.to_string();
                    if const_traits.contains(&trait_ident) {
                        if impl_item.generics.params.is_empty() {
                            if !processed_traits.contains(&trait_ident) {
                                let new_trait_path = syn::Path::from(Ident::new(&trait_ident, Span::call_site()));
                                let new_impl = parse_quote! {
                                    const impl<T> #new_trait_path for T {}
                                };
                                file.items.insert(i + 1, Item::Impl(new_impl));
                                processed_traits.insert(trait_ident);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a generic implementation `impl<T> const Trait for T` immediately after the first specific implementation of a `#[const_trait]` trait. This creates overlapping impl blocks that may trigger internal compiler errors when resolving defaultness of parent items, exploiting the interaction between generic and specific implementations under `#[const_trait]` and `min_specialization`."
    }
}