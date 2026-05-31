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

pub struct Add_Generic_Const_Parameter_320;

impl Mutator for Add_Generic_Const_Parameter_320 {
    fn name(&self) -> &str {
        "Add_Generic_Const_Parameter_320"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_traits = Vec::new();

        // Process traits with #[const_trait]
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if trait_item.attrs.iter().any(|attr| attr.path.is_ident("const_trait")) {
                    const_traits.push(trait_item.ident.clone());
                    // Process each method in the trait
                    for trait_item in &mut trait_item.items {
                        if let syn::TraitItem::Fn(method) = trait_item {
                            let generic = parse_quote!(const C: bool);
                            method.sig.generics.params.push(generic);
                        }
                    }
                }
            }
        }

        // Process const impls of the const traits
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some(constness) = &impl_item.constness {
                    if matches!(constness, syn::Constness::Const) {
                        if let Some((_, trait_path, _)) = &impl_item.trait_ {
                            if let Some(trait_ident) = trait_path.get_ident() {
                                if const_traits.iter().any(|t| t == trait_ident) {
                                    // Process each method in the impl
                                    for impl_item in &mut impl_item.items {
                                        if let syn::ImplItem::Fn(method) = impl_item {
                                            let generic = parse_quote!(const C: bool);
                                            method.sig.generics.params.push(generic);
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