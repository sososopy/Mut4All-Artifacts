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

pub struct Replace_Future_Assoc_With_Cyclic_Wrapper_125;

impl Mutator for Replace_Future_Assoc_With_Cyclic_Wrapper_125 {
    fn name(&self) -> &str {
        "Replace_Future_Assoc_With_Cyclic_Wrapper_125"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut wrapper_inserted = false;
        let mut wrapper_name = Ident::new("CyclicWrapper", Span::call_site());
        let mut wrapper_lifetime_name = Ident::new("a", Span::call_site());
        let mut wrapper_type_name = Ident::new("T", Span::call_site());
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &trait_item.items {
                    if let TraitItem::Type(type_item) = trait_item {
                        let mut has_future_bound = false;
                        for bound in &type_item.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.segments.last().map(|seg| seg.ident == "Future").unwrap_or(false) {
                                    has_future_bound = true;
                                    break;
                                }
                            }
                        }
                        if has_future_bound {
                            let trait_name = trait_item.ident.clone();
                            for item in &mut file.items {
                                if let Item::Impl(impl_item) = item {
                                    if impl_item.trait_.as_ref().map(|(_, path)| path.segments.last().map(|seg| seg.ident == trait_name).unwrap_or(false)).unwrap_or(false) {
                                        for impl_item in &mut impl_item.items {
                                            if let ImplItem::Type(type_item) = impl_item {
                                                if type_item.ident == trait_name {
                                                    let concrete_type = impl_item.self_ty.clone();
                                                    let lifetimes = impl_item.generics.lifetimes().map(|l| l.lifetime.clone()).collect::<Vec<_>>();
                                                    let has_lifetimes = lifetimes.len() > 0;
                                                    let wrapper_struct = if has_lifetimes {
                                                        parse_quote! {
                                                            struct #wrapper_name<'#wrapper_lifetime_name, #wrapper_type_name>(std::marker::PhantomData<&'#wrapper_lifetime_name #wrapper_type_name>);
                                                        }
                                                    } else {
                                                        parse_quote! {
                                                            struct #wrapper_name<#wrapper_type_name>(std::marker::PhantomData<#wrapper_type_name>);
                                                        }
                                                    };
                                                    let wrapper_future_impl = if has_lifetimes {
                                                        parse_quote! {
                                                            impl<'#wrapper_lifetime_name, #wrapper_type_name> std::future::Future for #wrapper_name<'#wrapper_lifetime_name, #wrapper_type_name> {
                                                                type Output = (#wrapper_type_name,);
                                                                fn poll(self: std::pin::Pin<&mut Self>, cx: &mut std::task::Context) -> std::task::Poll<Self::Output> {
                                    std::task::Poll::Pending
                                }
                            }
                        }
                                                    } else {
                                                        parse_1quote! {
                                                            impl<#wrapper_type_name> std::future::Future for #wrapper_name<#wrapper_type_name> {
                                                                type Output = (#wrapper_type_name,);
                                                                fn poll(self: std::pin::Pin<&mut Self>, cx: &mut std::task::Context) -> std::task::Poll<Self::Output> {
                                    std::task::Poll::Pending
                                }
                            }
                        }
                                                    };
                                                    if !wrapper_inserted {
                                                        file.items.push(Item::Struct(wrapper_struct));
                                                        file.items.push(Item::Impl(wrapper_future_impl));
                                                        wrapper_inserted = true;
                                                    }
                                                    let new_assoc_type = if has_lifetimes {
                                                        parse_quote! {
                                                            type #trait_name = #wrapper_name<'#wrapper_lifetime_name, #concrete_type>;
                                                        }
                                                    } else {
                                                        parse_quote! {
                                                            type #trait_name = #wrapper_name<#concrete_type>;
                                                        }
                                                    };
                                                    type_item.ty = Box::new(new_assoc_type);
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies traits with associated types bound to Future, finds implementations of those traits, and replaces the associated type binding with a new cyclic wrapper struct. The wrapper struct is defined with PhantomData and implements Future with Output containing the implementing type, creating a self-referential dependency. This mimics the bug's structure where a future's2 output is the implementing type, potentially causing infinite recursion during compiler trait resolution and inducing ICEs."
    }
}