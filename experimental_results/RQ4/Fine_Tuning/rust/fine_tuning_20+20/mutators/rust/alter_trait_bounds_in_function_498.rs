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

pub struct Alter_Trait_Bounds_In_Function_498;

impl Mutator for Alter_Trait_Bounds_In_Function_498 {
    fn name(&self) -> &str {
        "Alter_Trait_Bounds_In_Function_498"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_extra = false;
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                if trait_item.ident == "Extra" {
                    has_extra = true;
                    break;
                }
            }
        }
        if !has_extra {
            file.items
                .insert(0, syn::parse_quote!(trait Extra {}));
        }
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut has_target = false;
                let mut target_index = 0;
                for (i, param) in func.sig.generics.params.iter().enumerate() {
                    if let syn::GenericParam::Type(type_param) = param {
                        for bound in &type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Output") {
                                    has_target = true;
                                    target_index = i;
                                    break;
                                }
                            }
                        }
                    }
                }
                if !has_target {
                    continue;
                }
                if let syn::GenericParam::Type(type_param) =
                    &mut func.sig.generics.params[target_index]
                {
                    type_param.bounds.push(syn::parse_quote!(Extra));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut has_target = false;
                        let mut target_index = 0;
                        for (i, param) in func.sig.generics.params.iter().enumerate() {
                            if let syn::GenericParam::Type(type_param) = param {
                                for bound in &type_param.bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.is_ident("Output") {
                                            has_target = true;
                                            target_index = i;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if !has_target {
                            continue;
                        }
                        if let syn::GenericParam::Type(type_param) =
                            &mut func.sig.generics.params[target_index]
                        {
                            type_param.bounds.push(syn::parse_quote!(Extra));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify functions with a generic parameter bound to a trait like `Output`. Add an additional trait bound `Extra` to this parameter. This tests the compiler's trait resolution by introducing a new, potentially conflicting requirement to the existing trait bounds."
    }
}