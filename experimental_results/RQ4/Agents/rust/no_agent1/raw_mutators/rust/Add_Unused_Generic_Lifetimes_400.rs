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

pub struct Add_Unused_Generic_Lifetimes_400;

impl Mutator for Add_Unused_Generic_Lifetimes_400 {
    fn name(&self) -> &str {
        "Add_Unused_Generic_Lifetimes_400"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let generics = &mut func.sig.generics;
                    generics.params.push(parse_quote!('unused_lifetime1));
                    generics.params.push(parse_quote!('unused_lifetime2));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            generics.params.push(parse_quote!('unused_lifetime1));
                            generics.params.push(parse_quote!('unused_lifetime2));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused generic lifetimes to non-main function and impl signatures. By introducing lifetimes that are not utilized within the function or method, it tests the compiler's handling of lifetime parameters, potentially uncovering issues in lifetime resolution or unused parameter warnings."
    }
}