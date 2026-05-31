use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    str::FromStr,
};
use syn::{
    parse_quote,
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

pub struct Modify_Const_Trait_Implementation_324;

impl Mutator for Modify_Const_Trait_Implementation_324 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Implementation_324"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut has_const_trait_impl = false;

        for attr in &file.attrs {
            if attr.path().is_ident("feature") {
                if let Ok(syn::Meta::List(meta_list)) = attr.parse_meta() {
                    for nested_meta in meta_list.nested.iter() {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested_meta {
                            if path.is_ident("const_trait_impl") {
                                has_const_trait_impl = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if !has_const_trait_impl {
            return;
        }

        let mut target_trait_index = None;
        let mut target_impl_index = None;

        for (index, item) in file.items.iter().enumerate() {
            if let syn::Item::Trait(trait_item) = item {
                if trait_item
                    .attrs
                    .iter()
                    .any(|attr| attr.path().is_ident("const_trait"))
                {
                    for (impl_index, item) in file.items.iter().enumerate() {
                        if let syn::Item::Impl(impl_item) = item {
                            if impl_item.trait_.as_ref().map_or(false, |(_, path, _)| {
                                path.segments.last().map_or(false, |seg| {
                                    seg.ident == trait_item.ident
                                })
                            }) && impl_item.constness.is_some()
                            {
                                target_trait_index = Some(index);
                                target_impl_index = Some(impl_index);
                                break;
                            }
                        }
                    }
                }
            }
        }

        if target_trait_index.is_none() || target_impl_index.is_none() {
            for (index, item) in file.items.iter().enumerate() {
                if let syn::Item::Trait(trait_item) = item {
                    for (impl_index, item) in file.items.iter().enumerate() {
                        if let syn::Item::Impl(impl_item) = item {
                            if impl_item.trait_.as_ref().map_or(false, |(_, path, _)| {
                                path.segments.last().map_or(false, |seg| {
                                    seg.ident == trait_item.ident
                                })
                            }) && impl_item.constness.is_some()
                            {
                                target_trait_index = Some(index);
                                target_impl_index = Some(impl_index);
                                break;
                            }
                        }
                    }
                }
            }
        }

        if let Some(trait_index) = target_trait_index {
            if let syn::Item::Trait(trait_item) = &mut file.items[trait_index] {
                if !trait_item
                    .attrs
                    .iter()
                    .any(|attr| attr.path().is_ident("const_trait"))
                {
                    trait_item.attrs.push(parse_quote!(#[const_trait]));
                }
            }
        }

        if let Some(impl_index) = target_impl_index {
            if let syn::Item::Impl(impl_item) = &mut file.items[impl_index] {
                if let Some((_, path, _)) = &impl_item.trait_ {
                    let trait_name = path.segments.last().unwrap().ident.to_string();
                    let mut has_non_const_method = false;

                    for item in &impl_item.items {
                        if let syn::ImplItem::Fn(method) = item {
                            if !method.sig.inputs.iter().any(|arg| {
                                if let syn::FnArg::Typed(pat_type) = arg {
                                    if let syn::Type::TraitObject(trait_obj) = &*pat_type.ty {
                                        trait_obj.bounds.iter().any(|bound| {
                                            if let syn::TypeParamBound::Trait(trait_bound) = bound
                                            {
                                                trait_bound
                                                    .modifier
                                                    .as_ref()
                                                    .map_or(false, |modif| {
                                                        modif == &syn::TraitBoundModifier::TildeConst
                                                    })
                                            } else {
                                                false
                                            }
                                        })
                                    } else {
                                        false
                                    }
                                } else {
                                    false
                                }
                            }) && !method.sig.output.is_const_trait_bound()
                            {
                                has_non_const_method = true;
                                break;
                            }
                        }
                    }

                    if !has_non_const_method {
                        if let Some(last_item) = impl_item.items.last() {
                            if let syn::ImplItem::Fn(method) = last_item {
                                let new_method: syn::ImplItemFn = parse_quote! {
                                    fn new_method<A: ~const #trait_name>(&self) -> i32 {
                                        *self + 2
                                    }
                                };
                                impl_item.items.push(syn::ImplItem::Fn(new_method));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "Identify a const trait implementation and ensure the trait is marked with #[const_trait]. If the implementation lacks a method with ~const in its signature, add a new method with a ~const constraint on its parameter or return type."
    }
}

trait ConstTraitBoundExt {
    fn is_const_trait_bound(&self) -> bool;
}

impl ConstTraitBoundExt for syn::ReturnType {
    fn is_const_trait_bound(&self) -> bool {
        match self {
            syn::ReturnType::Default => false,
            syn::ReturnType::Type(_, ty) => {
                if let syn::Type::TraitObject(trait_obj) = &**ty {
                    trait_obj.bounds.iter().any(|bound| {
                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                            trait_bound
                                .modifier
                                .as_ref()
                                .map_or(false, |modif| modif == &syn::TraitBoundModifier::TildeConst)
                        } else {
                            false
                        }
                    })
                } else {
                    false
                }
            }
        }
    }
}