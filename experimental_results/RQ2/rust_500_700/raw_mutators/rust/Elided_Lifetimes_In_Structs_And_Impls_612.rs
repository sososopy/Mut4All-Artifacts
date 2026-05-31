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

pub struct Elided_Lifetimes_In_Structs_And_Impls_612;

impl Mutator for Elided_Lifetimes_In_Structs_And_Impls_612 {
    fn name(&self) -> &str {
        "Elided_Lifetimes_In_Structs_And_Impls_612"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(fields) = &mut item_struct.fields.iter().next() {
                    if let syn::Type::Reference(type_ref) = &fields.ty {
                        if type_ref.lifetime.is_none() {
                            let lifetime = Lifetime::new("'a", Span::call_site());
                            let lifetime_param = LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: lifetime.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            };
                            item_struct.generics.params.push(GenericParam::Lifetime(lifetime_param));
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                if item_impl.generics.params.is_empty() {
                    let lifetime = Lifetime::new("'b", Span::call_site());
                    let lifetime_param = LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: lifetime.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    };
                    item_impl.generics.params.push(GenericParam::Lifetime(lifetime_param));
                }
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.generics.params.is_empty() {
                            let lifetime = Lifetime::new("'c", Span::call_site());
                            let lifetime_param = LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: lifetime.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            };
                            func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                        }
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let syn::Type::Reference(type_ref) = &mut *pat_type.ty {
                                    if type_ref.lifetime.is_none() {
                                        type_ref.lifetime = Some(Lifetime::new("'c", Span::call_site()));
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
        ""
    }
}