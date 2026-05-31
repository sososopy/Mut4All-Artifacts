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

pub struct Add_Future_Generic_To_Trait_Impl_125;

impl Mutator for Add_Future_Generic_To_Trait_Impl_125 {
    fn name(&self) -> &str {
        "Add_Future_Generic_To_Trait_Impl_125"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    let mut has_future = false;
                    for seg in &trait_path.segments {
                        if seg.ident == "Future" {
                            has_future = true;
                            break;
                        }
                    }
                    if !has_future {
                        continue;
                    }
                }
                let mut found_future = false;
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(impl_type) = impl_item {
                        if impl_type.ident == "Future" {
                            found_future = true;
                            break;
                        }
                    }
                }
                if !found_future {
                    continue;
                }
                let mut found_target = false;
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(impl_fn) = impl_item {
                        if let ReturnType::Type(_, return_type) = &impl_fn.sig.output {
                            if let Type::Path(type_path) = &**return_type {
                                for seg in &type_path.path.segments {
                                    if seg.ident == "Future" {
                                        found_target = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if !found_target {
                    continue;
                }
                let new_generic: GenericParam = parse_quote!(T);
                item_impl.generics.params.push(new_generic);
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(impl_fn) = impl_item {
                        if let ReturnType::Type(_, return_type) = &mut impl_fn.sig.output {
                            if let Type::Path(type_path) = &mut **return_type {
                                type_path.path.segments.push(parse_quote!(T));
                            }
                        }
                        impl_fn.sig.generics.params.push(parse_quote!(T: Default));
                    }
                    if let syn::ImplItem::Type(impl_type) = impl_item {
                        if impl_type.ident == "Future" {
                            if let Type::Path(type_path) = &mut *impl_type.ty {
                                type_path.path.segments.push(parse_quote!(T));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations that define a `Future` associated type and have a method returning a future. It adds a new generic parameter `T` with a `Default` bound to the implementation and modifies the future type and method signature to include this new parameter. The transformation stresses the compiler's type inference and trait resolution systems by introducing additional generic complexity, potentially exposing bugs in how the compiler handles generic parameters in trait implementations."
    }
}