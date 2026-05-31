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

pub struct Add_Unused_Generic_Params_161;

impl Mutator for Add_Unused_Generic_Params_161 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Params_161"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    generics.params.push(parse_quote!(T: std::fmt::Debug));
                    generics.params.push(parse_quote!(U: std::clone::Clone));
                    generics.params.push(parse_quote!(V: std::cmp::PartialOrd));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            generics.params.push(parse_quote!(T: std::fmt::Debug));
                            generics.params.push(parse_quote!(U: std::clone::Clone));
                            generics.params.push(parse_quote!(V: std::cmp::PartialOrd));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused generic parameters with trait bounds (Debug, Clone, PartialOrd) to non-main function and impl signatures. This transformation introduces additional complexity and potential for type resolution conflicts, aiming to test the compiler's handling of redundant generics and trait bounds, potentially leading to ICEs or other deep compiler bugs."
    }
}