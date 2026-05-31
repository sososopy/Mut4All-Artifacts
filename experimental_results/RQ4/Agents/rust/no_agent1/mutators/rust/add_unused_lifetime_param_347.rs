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

pub struct Add_Unused_Lifetime_Param_347;

impl Mutator for Add_Unused_Lifetime_Param_347 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Param_347"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_param: Lifetime = parse_quote!('unused);
                func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam::new(lifetime_param.clone())));
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let syn::Type::Path(type_path) = &mut **ty {
                        if type_path.qself.is_none() {
                            type_path.path.segments.iter_mut().for_each(|segment| {
                                if let PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                    args.args.push(GenericArgument::Lifetime(Lifetime::new("'unused", Span::call_site())));
                                }
                            });
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetime_param: Lifetime = parse_quote!('unused);
                        func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam::new(lifetime_param.clone())));
                        if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                            if let syn::Type::Path(type_path) = &mut **ty {
                                if type_path.qself.is_none() {
                                    type_path.path.segments.iter_mut().for_each(|segment| {
                                        if let PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                            args.args.push(GenericArgument::Lifetime(Lifetime::new("'unused", Span::call_site())));
                                        }
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter `'unused` to the generics of functions and methods. It also attempts to apply this lifetime to the return type if it's a path type with angle-bracketed arguments. This mutation introduces a lifetime that doesn't correspond to any actual data, potentially leading to ICEs or lifetime resolution failures by stressing the borrow checker and lifetime inference mechanisms."
    }
}