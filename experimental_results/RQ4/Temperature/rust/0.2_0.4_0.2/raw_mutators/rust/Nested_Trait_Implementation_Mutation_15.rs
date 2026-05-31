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
};

use crate::mutator::Mutator;

pub struct Nested_Trait_Implementation_Mutation_15;

impl Mutator for Nested_Trait_Implementation_Mutation_15 {
    fn name(&self) -> &str {
        "Nested_Trait_Implementation_Mutation_15"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &**ty {
                        let mut new_bounds = type_impl_trait.bounds.clone();
                        new_bounds.push(parse_quote!(Clone));
                        *ty = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                            impl_token: type_impl_trait.impl_token,
                            bounds: new_bounds,
                        }));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let syn::Type::ImplTrait(type_impl_trait) = &**ty {
                                let mut new_bounds = type_impl_trait.bounds.clone();
                                new_bounds.push(parse_quote!(Clone));
                                *ty = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                                    impl_token: type_impl_trait.impl_token,
                                    bounds: new_bounds,
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions and methods with `impl Trait` return types, adding an additional `Clone` trait bound to the nested `impl Trait`. This transformation tests the compiler's handling of complex trait bounds and trait coercion, potentially revealing bugs in trait resolution and unsized type handling."
    }
}