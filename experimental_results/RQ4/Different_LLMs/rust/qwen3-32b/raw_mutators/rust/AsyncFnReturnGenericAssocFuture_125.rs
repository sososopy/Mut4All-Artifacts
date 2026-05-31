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

pub struct AsyncFnReturnGenericAssocFuture_125;

impl Mutator for AsyncFnReturnGenericAssocFuture_125 {
    fn name(&self) -> &str {
        "AsyncFnReturnGenericAssocFuture_125"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(_) = **return_type {
                        if let Some(first_param) = func.sig.generics.params.iter_mut().find(|param| matches!(param, syn::GenericParam::Type(_))) {
                            if let syn::GenericParam::Type(type_param) = first_param {
                                let trait_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                                    path: syn::Path::from(Ident::new("AsyncTrait", Span::call_site())),
                                    modifier: syn::TraitBoundModifier::None,
                                    paren_token: None,
                                    lifetimes: None,
                                });
                                type_param.bounds.push(trait_bound);
                                let param_name = &type_param.ident;
                                let future_type = parse_quote! { #param_name::Future };
                                *return_type = Box::new(future_type);
                                func.sig.asyncness = Some(token::Async {
                                    span: Span::call_site(),
                                });
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