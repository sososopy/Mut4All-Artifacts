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

pub struct Add_Unused_Generic_Params_410;

impl Mutator for Add_Unused_Generic_Params_410 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Params_410"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    generics.params.push(parse_quote!(T: std::fmt::Debug));
                    generics.params.push(parse_quote!(U: std::fmt::Display));
                    generics.params.push(parse_quote!(V: Clone));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            generics.params.push(parse_quote!(T: std::fmt::Debug));
                            generics.params.push(parse_quote!(U: std::fmt::Display));
                            generics.params.push(parse_quote!(V: Clone));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused generic parameters with trait bounds (Debug, Display, Clone) to non-main function and impl signatures. This transformation introduces complexity in the type system without affecting the function's logic, potentially triggering issues in the compiler's handling of generics, trait bounds, and type inference."
    }
}