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

pub struct Add_Unused_Generic_Params_73;

impl Mutator for Add_Unused_Generic_Params_73 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Params_73"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let generics = &mut func.sig.generics;
                    let unused_params: Vec<GenericParam> = (0..5).map(|i| {
                        let ident = Ident::new(&format!("U{}", i), Span::call_site());
                        parse_quote!(#ident)
                    }).collect();
                    generics.params.extend(unused_params);
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            let generics = &mut func.sig.generics;
                            let unused_params: Vec<GenericParam> = (0..5).map(|i| {
                                let ident = Ident::new(&format!("U{}", i), Span::call_site());
                                parse_quote!(#ident)
                            }).collect();
                            generics.params.extend(unused_params);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a series of unused generic parameters (U0, U1, ..., U4) to non-main functions and impl blocks. This transformation introduces complexity into the generics system without affecting the function's logic directly, potentially exposing issues in the compiler's handling of unused generics, type inference, and related optimizations."
    }
}