use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashMap, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Generic_Const_In_Trait_Bound_With_Const_Param_355;

impl Mutator for Generic_Const_In_Trait_Bound_With_Const_Param_355 {
    fn name(&self) -> &str {
        "Generic_Const_In_Trait_Bound_With_Const_Param_355"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_indices = HashMap::new();
        for (i, item) in file.items.iter().enumerate() {
            if let Item::Trait(trait_item) = item {
                trait_indices.insert(trait_item.ident.clone(), i);
            }
        }

        let mut impls_to_process = Vec::new();
        for (i, item) in file.items.iter().enumerate() {
            if let Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    let trait_name = trait_path.segments.last().unwrap().ident.clone();
                    if let Some(&trait_index) = trait_indices.get(&trait_name) {
                        impls_to_process.push((i, trait_index));
                    }
                }
            }
        }

        // Process traits first
        for (_, trait_index) in &impls_to_process {
            if let Some(trait_item) = file.items.get_mut(trait_index) {
                if let Item::Trait(trait_item) = trait_item {
                    let mut has_type_param = false;
                    for param in &trait_item.generics.params {
                        if let GenericParam::Type(_) = param {
                            has_type_param = true;
                            break;
                        }
                    }
                    if !has_type_param {
                        trait_item.generics.params.push(parse_quote!(T));
                    }
                }
            }
        }

        // Process impls
        for (impl_index, trait_index) in &impls_to_process {
            if let Some(impl_item) = file.items.get_mut(impl_index) {
                if let Item::Impl(impl_item) = impl_item {
                    let mut impl_has_type_param = false;
                    for param in &impl_item.generics.params {
                        if let GenericParam::Type(_) = param {
                            impl_has_type_param = true;
                            break;
                        }
                    }
                    if !impl_has_type_param {
                        impl_item.generics.params.push(parse_quote!(T));
                    }
                    let array_type = parse_quote! {[(); std::mem::size_of::<T>()]};
                    let mut where_clause = impl_item.generics.where_clause.get_or_insert_with(|| {
                        WhereClause {
                            where_token: Default::default(),
                            predicates: Punctuated::new(),
                        }
                    });
                    let predicate = syn::WherePredicate::Type(syn::PredicateType {
                        bounded_ty: array_type,
                        colon_token: token::Colon::default(),
                        bounds: Punctuated::from_iter(vec![syn::TypeParamBound::Trait(syn::TraitBound {
                            path: syn::Path::from(Ident::new("Sized", Span::call_site())),
                            modifier: syn::TraitBoundModifier::None,
                            paren_token: None,
                            lifetimes: None,
                        })]),
                        lifetimes: None,
                    });
                    where_clause.predicates.push(predicate);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}