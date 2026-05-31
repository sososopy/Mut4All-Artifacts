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

pub struct Add_Unused_Lifetime_Parameter_149;

impl Mutator for Add_Unused_Lifetime_Parameter_149 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Parameter_149"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'unused", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let lifetime_param = LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'unused", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        };
                        func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter `'unused` to the generics of non-main function and impl signatures. This transformation introduces redundant lifetimes, potentially leading to lifetime resolution issues or ICEs when the compiler attempts to verify lifetime constraints that are not actually used in the function body or signature."
    }
}