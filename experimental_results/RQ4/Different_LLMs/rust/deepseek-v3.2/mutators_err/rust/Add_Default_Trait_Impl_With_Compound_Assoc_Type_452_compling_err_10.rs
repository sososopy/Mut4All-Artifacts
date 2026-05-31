use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Default_Trait_Impl_With_Compound_Assoc_Type_452;

impl Mutator for Add_Default_Trait_Impl_With_Compound_Assoc_Type_452 {
    fn name(&self) -> &str {
        "Add_Default_Trait_Impl_With_Compound_Assoc_Type_452"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_with_assoc = None;
        let mut other_trait = None;
        let mut has_specialization = false;

        for attr in &file.attrs {
            if let Some(ident) = attr.path().get_ident() {
                if ident == "feature" {
                    if let syn::Meta::List(list) = &attr.meta {
                        if list.path.is_ident("feature") {
                            let nested = attr.parse_args_with(Punctuated::<syn::Meta, token::Comma>::parse_terminated);
                            if let Ok(nested) = nested {
                                for meta in nested {
                                    if let syn::Meta::Path(path) = meta {
                                        if path.is_ident("specialization") {
                                            has_specialization = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                let mut has_assoc_type = false;
                for item in &item_trait.items {
                    if let syn::TraitItem::Type(_) = item {
                        has_assoc_type = true;
                        break;
                    }
                }
                if has_assoc_type {
                    if trait_with_assoc.is_none() {
                        trait_with_assoc = Some((item_trait.ident.clone(), item_trait.unsafety.is_some()));
                    }
                } else {
                    if other_trait.is_none() {
                        other_trait = Some(item_trait.ident.clone());
                    }
                }
            }
        }

        if trait_with_assoc.is_none() {
            return;
        }

        let (trait_a_ident, is_unsafe) = trait_with_assoc.unwrap();
        let trait_b_ident = if let Some(ident) = other_trait {
            ident
        } else {
            let new_trait_ident = Ident::new("Marker", Span::call_site());
            let new_trait: ItemTrait = parse_quote! {
                trait Marker {}
            };
            file.items.push(Item::Trait(new_trait));
            new_trait_ident
        };

        if !has_specialization {
            let feature_attr: syn::Attribute = parse_quote! {
                #![feature(specialization)]
            };
            file.attrs.insert(0, feature_attr);
        }

        let default_impl: syn::ItemImpl = if is_unsafe {
            parse_quote! {
                unsafe impl<X> #trait_a_ident for X where X: #trait_b_ident {}
            }
        } else {
            parse_quote! {
                impl<X> #trait_a_ident for X where X: #trait_b_ident {}
            }
        };
        file.items.push(Item::Impl(default_impl));

        let mut found_function = false;
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_trait_a_bound = false;
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(segment) = trait_bound.path.segments.last() {
                                    if segment.ident == trait_a_ident {
                                        has_trait_a_bound = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if has_trait_a_bound {
                            break;
                        }
                    }
                }
                if has_trait_a_bound {
                    for input in &mut item_fn.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            let type_path_clone;
                            if let Type::Path(type_path) = &*pat_type.ty {
                                type_path_clone = type_path.clone();
                                if let Some(segment) = type_path_clone.path.segments.last() {
                                    if segment.ident == "Handle" {
                                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                            for arg in &args.args {
                                                if let GenericArgument::Type(Type::Path(arg_path)) = arg {
                                                    if let Some(arg_segment) = arg_path.path.segments.last() {
                                                        if arg_segment.ident == trait_a_ident {
                                                            pat_type.ty = Box::new(parse_quote! { (#type_path_clone,) });
                                                            found_function = true;
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
                    if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                        let type_path_clone;
                        if let Type::Path(type_path) = &**return_type {
                            type_path_clone = type_path.clone();
                            if let Some(segment) = type_path_clone.path.segments.last() {
                                if segment.ident == "Handle" {
                                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                        for arg in &args.args {
                                            if let GenericArgument::Type(Type::Path(arg_path)) = arg {
                                                if let Some(arg_segment) = arg_path.path.segments.last() {
                                                    if arg_segment.ident == trait_a_ident {
                                                        *return_type = Box::new(parse_quote! { (#type_path_clone,) });
                                                        found_function = true;
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

        if !found_function {
            let new_function: ItemFn = parse_quote! {
                fn mutate_op<S: #trait_a_ident>(boxed: (S::Handle,)) -> (S::Handle,) {
                    boxed
                }
            };
            file.items.push(Item::Fn(new_function));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a default implementation for a trait with an associated type, conditional on another trait bound, and modifies function signatures to use the associated type within compound types. It first identifies a trait with an associated type (TraitA) and another distinct trait (TraitB). If the specialization feature is not present, it adds it. Then it creates a default impl<T: TraitB> TraitA for T, marked unsafe if TraitA is unsafe. Finally, it locates or creates a function bounded by TraitA and changes its signature to use a tuple containing the associated type projection (e.g., (S::Handle,)). This transformation tests the compiler's handling of default trait implementations, associated types in compound contexts, and trait specialization interactions."
    }
}