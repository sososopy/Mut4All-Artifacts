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

pub struct Add_Unused_Lifetime_Bounds_446;

impl Mutator for Add_Unused_Lifetime_Bounds_446 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Bounds_446"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    let unused_lifetime = Lifetime::new("'unused", Span::call_site());
                    generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: unused_lifetime.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    if let Some(where_clause) = &mut generics.where_clause {
                        where_clause.predicates.push(parse_quote!('unused: 'static));
                    } else {
                        generics.where_clause = Some(parse_quote!(where 'unused: 'static));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            let unused_lifetime = Lifetime::new("'unused", Span::call_site());
                            generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: unused_lifetime.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            }));
                            if let Some(where_clause) = &mut generics.where_clause {
                                where_clause.predicates.push(parse_quote!('unused: 'static));
                            } else {
                                generics.where_clause = Some(parse_quote!(where 'unused: 'static));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter `'unused` with a `'static` bound to non-main function and impl signatures. This transformation introduces unnecessary complexity into the lifetime system, potentially provoking issues in lifetime resolution and inference, especially in complex generic contexts."
    }
}