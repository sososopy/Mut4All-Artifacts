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

pub struct Async_Function_With_Multiple_Named_Lifetimes_22;

impl Mutator for Async_Function_With_Multiple_Named_Lifetimes_22 {
    fn name(&self) -> &str {
        "Async_Function_With_Multiple_Named_Lifetimes_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_none() {
                    let mut lifetimes: Vec<&Lifetime> = vec![];
                    for input in &func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(ref_type) = &*pat_type.ty {
                                if let Some(lt) = &ref_type.lifetime {
                                    lifetimes.push(lt);
                                }
                            }
                        }
                    }
                    let unique_lifetimes: HashSet<_> = lifetimes.iter().collect();
                    if unique_lifetimes.len() >= 2 {
                        func.sig.asyncness = Some(syn::token::Async {
                            span: proc_macro2::Span::call_site(),
                        });
                        for lt in &unique_lifetimes {
                            let lt_ident = &lt.ident;
                            let mut found = false;
                            for param in &func.sig.generics.params {
                                if let GenericParam::Lifetime(lt_param) = param {
                                    if lt_param.lifetime.ident == *lt_ident {
                                        found = true;
                                        break;
                                    }
                                }
                            }
                            if !found {
                                let new_lt = LifetimeParam {
                                    attrs: vec![],
                                    lifetime: Lifetime {
                                        apostrophe: proc_macro2::Span::call_site().into(),
                                        ident: lt.ident.clone(),
                                    },
                                    colon_token: None,
                                };
                                func.sig.generics.params.push(GenericParam::Lifetime(new_lt));
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