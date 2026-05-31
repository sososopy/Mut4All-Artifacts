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

pub struct Add_Unused_Lifetime_Constraints_306;

impl Mutator for Add_Unused_Lifetime_Constraints_306 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Constraints_306"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetimes = &mut func.sig.generics.params;
                lifetimes.push(GenericParam::Lifetime(LifetimeDef {
                    attrs: vec![],
                    lifetime: Lifetime::new("'unused", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let lifetimes = &mut func.sig.generics.params;
                        lifetimes.push(GenericParam::Lifetime(LifetimeDef {
                            attrs: vec![],
                            lifetime: Lifetime::new("'unused", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter to function and method signatures. This transformation introduces redundant lifetime constraints, potentially leading to ICEs or inference failures in the compiler's lifetime resolution system. It aims to stress-test the handling of lifetimes and uncover bugs related to lifetime management and elision."
    }
}