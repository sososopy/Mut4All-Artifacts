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

pub struct Replace_Lifetime_Parameter_383;

impl Mutator for Replace_Lifetime_Parameter_383 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameter_383"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    for param in generics.params.iter_mut() {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime_param = LifetimeParam {
                                attrs: lifetime_param.attrs.clone(),
                                lifetime: Lifetime {
                                    apostrophe: lifetime_param.lifetime.apostrophe,
                                    ident: Ident::new(&format!("'{}", thread_rng().r#gen::<u32>()), Span::call_site()),
                                },
                                bounds: lifetime_param.bounds.clone(),
                                colon_token: lifetime_param.colon_token,
                            };
                            *param = GenericParam::Lifetime(new_lifetime_param);
                        }
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in generics.params.iter_mut() {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime_param = LifetimeParam {
                                attrs: lifetime_param.attrs.clone(),
                                lifetime: Lifetime {
                                    apostrophe: lifetime_param.lifetime.apostrophe,
                                    ident: Ident::new(&format!("'{}", thread_rng().r#gen::<u32>()), Span::call_site()),
                                },
                                bounds: lifetime_param.bounds.clone(),
                                colon_token: lifetime_param.colon_token,
                            };
                            *param = GenericParam::Lifetime(new_lifetime_param);
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in generics.params.iter_mut() {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            let new_lifetime_param = LifetimeParam {
                                attrs: lifetime_param.attrs.clone(),
                                lifetime: Lifetime {
                                    apostrophe: lifetime_param.lifetime.apostrophe,
                                    ident: Ident::new(&format!("'{}", thread_rng().r#gen::<u32>()), Span::call_site()),
                                },
                                bounds: lifetime_param.bounds.clone(),
                                colon_token: lifetime_param.colon_token,
                            };
                            *param = GenericParam::Lifetime(new_lifetime_param);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing lifetime parameters in structs, functions, and impl blocks with new ones. It aims to test the compiler's ability to handle complex lifetime relationships and generic const expressions by introducing new lifetime parameters and replacing existing ones."
    }
}