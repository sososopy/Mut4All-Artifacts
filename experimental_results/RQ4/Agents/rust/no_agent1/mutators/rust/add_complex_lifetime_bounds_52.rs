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

pub struct Add_Complex_Lifetime_Bounds_52;

impl Mutator for Add_Complex_Lifetime_Bounds_52 {
    fn name(&self) -> &str {
        "Add_Complex_Lifetime_Bounds_52"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                let lifetime_a = Lifetime::new("'a", Span::call_site());
                let lifetime_b = Lifetime::new("'b", Span::call_site());
                let lifetime_c = Lifetime::new("'c", Span::call_site());

                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime_a.clone(),
                    colon_token: Some(token::Colon::default()),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(lifetime_b.clone());
                        bounds.push(lifetime_c.clone());
                        bounds
                    },
                }));

                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime_b.clone(),
                    colon_token: Some(token::Colon::default()),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(lifetime_c.clone());
                        bounds
                    },
                }));

                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime_c.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
            }

            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        let lifetime_a = Lifetime::new("'a", Span::call_site());
                        let lifetime_b = Lifetime::new("'b", Span::call_site());
                        let lifetime_c = Lifetime::new("'c", Span::call_site());

                        generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: lifetime_a.clone(),
                            colon_token: Some(token::Colon::default()),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(lifetime_b.clone());
                                bounds.push(lifetime_c.clone());
                                bounds
                            },
                        }));

                        generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: lifetime_b.clone(),
                            colon_token: Some(token::Colon::default()),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(lifetime_c.clone());
                                bounds
                            },
                        }));

                        generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: lifetime_c.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces complex lifetime bounds into function and method signatures. By creating intricate lifetime hierarchies, it challenges the compiler's ability to resolve lifetime relationships and enforce borrow checking rules, potentially leading to ICEs or inference failures in lifetime resolution."
    }
}