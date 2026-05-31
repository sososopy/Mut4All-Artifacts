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

pub struct Modify_Function_Keywords_In_Trait_Impl_316;

impl Mutator for Modify_Function_Keywords_In_Trait_Impl_316 {
    fn name(&self) -> &str {
        "Modify_Function_Keywords_In_Trait_Impl_316"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut keywords = Vec::new();
                        if func.sig.asyncness.is_some() {
                            keywords.push("async");
                        }
                        if func.sig.constness.is_some() {
                            keywords.push("const");
                        }
                        if func.sig.unsafety.is_some() {
                            keywords.push("unsafe");
                        }
                        if func.sig.abi.is_some() {
                            keywords.push("extern");
                        }
                        keywords.push("fn");

                        if keywords.contains(&"async") && keywords.contains(&"const") {
                            // Swap async and const
                            func.sig.asyncness = None;
                            func.sig.constness = None;
                            func.sig.constness = Some(token::Const { span: Span::call_site() });
                            func.sig.asyncness = Some(token::Async { span: Span::call_site() });
                        } else if keywords.contains(&"async") {
                            // Remove async
                            func.sig.asyncness = None;
                        } else if keywords.contains(&"const") {
                            // Remove const
                            func.sig.constness = None;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies function keywords within trait implementations by altering the order or removing specific keywords like `async` and `const`. This aims to test the compiler's handling of keyword combinations and their effects on function behavior, particularly in trait contexts where such modifications can lead to unexpected results or compiler errors."
    }
}