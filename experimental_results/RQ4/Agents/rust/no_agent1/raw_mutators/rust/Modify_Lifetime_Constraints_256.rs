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

pub struct Modify_Lifetime_Constraints_256;

impl Mutator for Modify_Lifetime_Constraints_256 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Constraints_256"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut new_lifetime = Lifetime::new("'a", Span::call_site());
                let lifetime_param = LifetimeParam {
                    attrs: vec![],
                    lifetime: new_lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::Reference(type_ref) = &mut **return_type {
                        type_ref.lifetime = Some(new_lifetime.clone());
                    }
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &mut *pat_type.ty {
                            type_ref.lifetime = Some(new_lifetime.clone());
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut new_lifetime = Lifetime::new("'b", Span::call_site());
                        let lifetime_param = LifetimeParam {
                            attrs: vec![],
                            lifetime: new_lifetime.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        };
                        func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                        if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                            if let Type::Reference(type_ref) = &mut **return_type {
                                type_ref.lifetime = Some(new_lifetime.clone());
                            }
                        }
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                    type_ref.lifetime = Some(new_lifetime.clone());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces new lifetime parameters into function and method signatures, and applies them to reference types in inputs and outputs. This transformation aims to stress the compiler's lifetime resolution system, potentially triggering ICEs or assertion failures due to complex or conflicting lifetime constraints."
    }
}