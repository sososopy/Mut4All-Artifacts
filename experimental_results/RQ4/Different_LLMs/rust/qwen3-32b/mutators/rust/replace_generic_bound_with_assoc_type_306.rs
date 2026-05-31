use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, parse_quote,
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

pub struct Replace_Generic_Bound_With_Assoc_Type_306;

impl Mutator for Replace_Generic_Bound_With_Assoc_Type_306 {
    fn name(&self) -> &str {
        "Replace_Generic_Bound_With_Assoc_Type_306"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let assoc_types: Vec<_> = trait_item
                    .items
                    .iter()
                    .filter_map(|item| {
                        if let syn::TraitItem::Type(ty) = item {
                            Some(ty.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if assoc_types.is_empty() {
                    continue;
                }
                let trait_name = &trait_item.ident;
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if method.sig.asyncness.is_some() {
                            let assoc_name = &assoc_types[0];
                            for param in &mut method.sig.generics.params {
                                if let syn::GenericParam::Type(type_param) = param {
                                    let new_bound = parse_quote! {
                                        #trait_name<#assoc_name = <Self as #trait_name>::#assoc_name>
                                    };
                                    type_param.bounds.push(new_bound);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async methods in traits that define an associated type. It adds a generic bound to each type parameter of the method, requiring it to satisfy the trait's associated type constraint using a recursive reference. This transformation forces the compiler to resolve complex binder references and may trigger ICEs related to unresolved type constraints in async contexts."
    }
}