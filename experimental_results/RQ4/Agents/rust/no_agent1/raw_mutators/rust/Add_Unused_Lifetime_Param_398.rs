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

pub struct Add_Unused_Lifetime_Param_398;

impl Mutator for Add_Unused_Lifetime_Param_398 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Param_398"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    let lifetime_param = syn::LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: syn::Lifetime::new("'unused", Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    };
                    generics.params.push(syn::GenericParam::Lifetime(lifetime_param));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            let lifetime_param = syn::LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: syn::Lifetime::new("'unused", Span::call_site()),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            };
                            generics.params.push(syn::GenericParam::Lifetime(lifetime_param));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter to non-main function and impl signatures. This transformation introduces unnecessary complexity and potential conflicts in lifetime resolution, testing the compiler's ability to handle redundant or conflicting lifetime parameters. It aims to provoke ICEs or assertion failures related to lifetime inference and resolution."
    }
}