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

pub struct Modify_Function_Signature_To_Introduce_Trait_Bounds_480;

impl Mutator for Modify_Function_Signature_To_Introduce_Trait_Bounds_480 {
    fn name(&self) -> &str {
        "Modify_Function_Signature_To_Introduce_Trait_Bounds_480"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut has_generic = false;
                let mut has_bound = false;
                for param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        has_generic = true;
                        if type_param.bounds.is_empty() {
                            type_param.bounds.push(parse_quote!(Clone));
                            has_bound = true;
                            break;
                        }
                    }
                }
                if has_generic && has_bound {
                    continue;
                }
                if has_generic && !has_bound {
                    for param in &mut func.sig.generics.params {
                        if let syn::GenericParam::Type(type_param) = param {
                            type_param.bounds.push(parse_quote!(Clone));
                            break;
                        }
                    }
                }
                if !has_generic {
                    func.sig
                        .generics
                        .params
                        .push(parse_quote!(T: Clone + Default));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut has_generic = false;
                        let mut has_bound = false;
                        for param in &mut func.sig.generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                has_generic = true;
                                if type_param.bounds.is_empty() {
                                    type_param.bounds.push(parse_quote!(Clone));
                                    has_bound = true;
                                    break;
                                }
                            }
                        }
                        if has_generic && has_bound {
                            continue;
                        }
                        if has_generic && !has_bound {
                            for param in &mut func.sig.generics.params {
                                if let syn::GenericParam::Type(type_param) = param {
                                    type_param.bounds.push(parse_quote!(Clone));
                                    break;
                                }
                            }
                        }
                        if !has_generic {
                            func.sig
                                .generics
                                .params
                                .push(parse_quote!(T: Clone + Default));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions by modifying their generic type parameters to introduce trait bounds. If a function has generics without bounds, it adds a `Clone` bound to one of them. If all generics already have bounds, it appends `Clone` to an existing generic. If no generics are present, it introduces a new generic parameter `T` with `Clone` and `Default` bounds. This transformation stresses the compiler's trait resolution and type inference systems by altering the constraints on generic parameters, potentially revealing bugs in how the compiler handles trait bounds and generic substitutions."
    }
}