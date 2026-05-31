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

pub struct Collapse_Async_Function_Lifetimes_39;

impl Mutator for Collapse_Async_Function_Lifetimes_39 {
    fn name(&self) -> &str {
        "Collapse_Async_Function_Lifetimes_39"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if !func.sig.asyncness.is_some() {
                    continue;
                }
                let mut lifetime_count = 0;
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Lifetime(_) = param {
                        lifetime_count += 1;
                    }
                }
                if lifetime_count < 2 {
                    continue;
                }
                let mut new_generics = func.sig.generics.clone();
                new_generics.params.clear();
                new_generics
                    .params
                    .push(syn::GenericParam::Lifetime(parse_quote!('a)));
                let mut new_inputs = Punctuated::new();
                for input in &func.sig.inputs {
                    match input {
                        syn::FnArg::Typed(pat_type) => {
                            let mut new_ty = pat_type.ty.clone();
                            if let syn::Type::Reference(mut ref_type) = *new_ty {
                                ref_type.lifetime = Some(parse_quote!('a));
                                new_ty = Box::new(syn::Type::Reference(ref_type));
                            }
                            new_inputs.push(syn::FnArg::Typed(syn::PatType {
                                attrs: pat_type.attrs.clone(),
                                pat: pat_type.pat.clone(),
                                colon_token: pat_type.colon_token,
                                ty: new_ty,
                            }));
                        }
                        _ => new_inputs.push(input.clone()),
                    }
                }
                let new_output = match &func.sig.output {
                    syn::ReturnType::Type(arrow, ty) => {
                        let mut new_ty = ty.clone();
                        if let syn::Type::Reference(mut ref_type) = *new_ty {
                            ref_type.lifetime = Some(parse_quote!('a));
                            new_ty = Box::new(syn::Type::Reference(ref_type));
                        }
                        syn::ReturnType::Type(*arrow, new_ty)
                    }
                    _ => func.sig.output.clone(),
                };
                func.sig.generics = new_generics;
                func.sig.inputs = new_inputs;
                func.sig.output = new_output;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets async functions with multiple lifetime parameters by collapsing them into a single lifetime. It rewrites the function's generic lifetimes, parameter types, and return type to use a unified lifetime, aiming to stress-test the borrow checker and async transformation logic for potential lifetime-related compiler bugs."
    }
}