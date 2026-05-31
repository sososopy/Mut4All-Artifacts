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

pub struct Add_Unused_Lifetime_113;

impl Mutator for Add_Unused_Lifetime_113 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_113"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                // Add an unused lifetime to function signatures
                let unused_lifetime: Lifetime = parse_quote!('unused);
                func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: unused_lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        // Add an unused lifetime to method signatures
                        let unused_lifetime: Lifetime = parse_quote!('unused);
                        func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: unused_lifetime.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter to function and method signatures. This transformation can lead to lifetime resolution issues, potentially triggering ICEs or other deep compiler bugs related to lifetime inference and checking. By introducing unused lifetimes, it stresses the compiler's handling of lifetime parameters and their interactions with existing code."
    }
}