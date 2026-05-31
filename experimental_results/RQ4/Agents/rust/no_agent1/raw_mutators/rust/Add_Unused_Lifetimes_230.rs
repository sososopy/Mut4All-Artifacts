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

pub struct Add_Unused_Lifetimes_230;

impl Mutator for Add_Unused_Lifetimes_230 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetimes_230"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetimes = &mut func.sig.generics.params;
                lifetimes.push(GenericParam::Lifetime(LifetimeDef {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'a", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
                lifetimes.push(GenericParam::Lifetime(LifetimeDef {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'b", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetimes = &mut func.sig.generics.params;
                        lifetimes.push(GenericParam::Lifetime(LifetimeDef {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'a", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                        lifetimes.push(GenericParam::Lifetime(LifetimeDef {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'b", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds unused lifetimes ('a and 'b) to function and impl method signatures. This transformation introduces unnecessary complexity into the lifetime system, potentially revealing issues in lifetime resolution and unused lifetime handling. It aims to stress-test the compiler's ability to manage and optimize lifetimes, especially in scenarios where they are superfluous."
    }
}