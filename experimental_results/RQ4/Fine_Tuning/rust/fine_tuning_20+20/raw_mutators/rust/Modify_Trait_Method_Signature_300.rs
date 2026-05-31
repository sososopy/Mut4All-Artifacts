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

pub struct Modify_Trait_Method_Signature_300;

impl Mutator for Modify_Trait_Method_Signature_300 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Signature_300"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = Vec::new();
        let mut trait_impls = Vec::new();
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                trait_names.push(trait_item.ident.to_string());
            }
        }
        for item in &file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = &impl_item.trait_ {
                    let trait_name = path.segments.last().unwrap().ident.to_string();
                    if trait_names.contains(&trait_name) {
                        trait_impls.push(trait_name);
                    }
                }
            }
        }
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut has_generic_method = false;
                for item in &trait_item.items {
                    if let syn::TraitItem::Fn(method) = item {
                        if !method.sig.generics.params.is_empty() {
                            has_generic_method = true;
                            break;
                        }
                    }
                }
                if !has_generic_method {
                    continue;
                }
                let mut has_async_generic_method = false;
                for item in &trait_item.items {
                    if let syn::TraitItem::Fn(method) = item {
                        if method.sig.asyncness.is_some()
                            && !method.sig.generics.params.is_empty()
                        {
                            has_async_generic_method = true;
                            break;
                        }
                    }
                }
                if has_async_generic_method {
                    for item in &mut trait_item.items {
                        if let syn::TraitItem::Fn(method) = item {
                            if method.sig.asyncness.is_some()
                                && !method.sig.generics.params.is_empty()
                            {
                                let new_generic: GenericParam = parse_quote! { T };
                                method.sig.generics.params.push(new_generic);
                            }
                        }
                    }
                } else {
                    for item in &mut trait_item.items {
                        if let syn::TraitItem::Fn(method) = item {
                            if !method.sig.generics.params.is_empty() {
                                let new_generic: GenericParam = parse_quote! { T };
                                method.sig.generics.params.push(new_generic);
                                break;
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = &impl_item.trait_ {
                    let trait_name = path.segments.last().unwrap().ident.to_string();
                    if trait_impls.contains(&trait_name) {
                        let mut has_generic_method = false;
                        for item in &impl_item.items {
                            if let syn::ImplItem::Fn(method) = item {
                                if !method.sig.generics.params.is_empty() {
                                    has_generic_method = true;
                                    break;
                                }
                            }
                        }
                        if !has_generic_method {
                            continue;
                        }
                        let mut has_async_generic_method = false;
                        for item in &impl_item.items {
                            if let syn::ImplItem::Fn(method) = item {
                                if method.sig.asyncness.is_some()
                                    && !method.sig.generics.params.is_empty()
                                {
                                    has_async_generic_method = true;
                                    break;
                                }
                            }
                        }
                        if has_async_generic_method {
                            for item in &mut impl_item.items {
                                if let syn::ImplItem::Fn(method) = item {
                                    if method.sig.asyncness.is_some()
                                        && !method.sig.generics.params.is_empty()
                                    {
                                        let new_generic: GenericParam = parse_quote! { T };
                                        method.sig.generics.params.push(new_generic);
                                    }
                                }
                            }
                        } else {
                            for item in &mut impl_item.items {
                                if let syn::ImplItem::Fn(method) = item {
                                    if !method.sig.generics.params.is_empty() {
                                        let new_generic: GenericParam = parse_quote! { T };
                                        method.sig.generics.params.push(new_generic);
                                        break;
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
        "The mutator targets traits with generic methods, prioritizing async methods. It adds an unconstrained generic parameter to the method signature, ensuring consistency across trait definitions and implementations. This transformation stresses the compiler's handling of generic parameters, trait method resolution, and async function semantics, aiming to uncover issues in type inference and trait system robustness."
    }
}