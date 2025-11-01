use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Modify_Function_Keywords_126;

impl Mutator for Modify_Function_Keywords_126 {
    fn name(&self) -> &str {
        "Modify_Function_Keywords_126"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Method(method) = impl_item {
                        let mut has_async = false;
                        let mut has_const = false;
                        let mut has_unsafe = false;
                        let mut has_extern = false;

                        method.sig.constness.take().map(|_| has_const = true);
                        method.sig.asyncness.take().map(|_| has_async = true);
                        method.sig.unsafety.take().map(|_| has_unsafe = true);
                        method.sig.abi.take().map(|_| has_extern = true);

                        if has_async && has_const {
                            if thread_rng().gen_bool(0.5) {
                                method.sig.constness = Some(token::Const(Span::call_site()));
                                method.sig.asyncness = Some(token::Async(Span::call_site()));
                            } else {
                                method.sig.constness = Some(token::Const(Span::call_site()));
                            }
                        } else if has_const && has_unsafe {
                            if thread_rng().gen_bool(0.5) {
                                method.sig.constness = Some(token::Const(Span::call_site()));
                                method.sig.unsafety = Some(token::Unsafe(Span::call_site()));
                            } else {
                                method.sig.constness = Some(token::Const(Span::call_site()));
                            }
                        }
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions within trait implementations, specifically modifying the order and presence of keywords such as async, const, unsafe, and extern. The goal is to explore potential compiler errors or unexpected behavior by altering these keyword combinations. The operator swaps async and const or removes one of them, focusing on trait implementations where such changes can lead to interesting compiler reactions."
    }
}