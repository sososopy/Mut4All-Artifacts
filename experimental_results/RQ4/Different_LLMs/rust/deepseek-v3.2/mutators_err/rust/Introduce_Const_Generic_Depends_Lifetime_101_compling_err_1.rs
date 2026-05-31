use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Introduce_Const_Generic_Depends_Lifetime_101;

impl Mutator for Introduce_Const_Generic_Depends_Lifetime_101 {
    fn name(&self) -> &str {
        "Introduce_Const_Generic_Depends_Lifetime_101"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_feature = false;
        for attr in &file.attrs {
            if let syn::AttrStyle::Outer = attr.style {
                if let syn::Meta::List(list) = &attr.meta {
                    if list.path.is_ident("feature") {
                        if let syn::Meta::NameValue(nv) = &list.nested.first().unwrap() {
                            if let syn::Expr::Lit(lit) = &nv.value {
                                if let syn::Lit::Str(lit_str) = &lit.lit {
                                    if lit_str.value() == "generic_const_exprs" {
                                        has_feature = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if !has_feature {
            file.attrs.insert(0, parse_quote!(#![feature(generic_const_exprs)]));
            file.attrs.insert(1, parse_quote!(#![allow(incomplete_features)]));
        }
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut has_lifetime = false;
                for param in &func.sig.generics.params {
                    if let syn::GenericParam::Lifetime(_) = param {
                        has_lifetime = true;
                        break;
                    }
                }
                if !has_lifetime {
                    continue;
                }
                let lifetime_param = func.sig.generics.params.iter().find_map(|p| {
                    if let syn::GenericParam::Lifetime(lp) = p {
                        Some(lp.lifetime.clone())
                    } else {
                        None
                    }
                });
                if let Some(lifetime) = lifetime_param {
                    let helper_name = Ident::new("helper_mutated", Span::call_site());
                    let helper_stmt: Stmt = parse_quote! {
                        const fn #helper_name<'b>() -> usize where &'b (): Sized { 3 }
                    };
                    func.block.stmts.insert(0, helper_stmt);
                    let array_stmt: Stmt = parse_quote! {
                        let _: [u8; #helper_name::<'a>()];
                    };
                    func.block.stmts.insert(1, array_stmt);
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut has_lifetime = false;
                        for param in &func.sig.generics.params {
                            if let syn::GenericParam::Lifetime(_) = param {
                                has_lifetime = true;
                                break;
                            }
                        }
                        if !has_lifetime {
                            continue;
                        }
                        let lifetime_param = func.sig.generics.params.iter().find_map(|p| {
                            if let syn::GenericParam::Lifetime(lp) = p {
                                Some(lp.lifetime.clone())
                            } else {
                                None
                            }
                        });
                        if let Some(lifetime) = lifetime_param {
                            let helper_name = Ident::new("helper_mutated", Span::call_site());
                            let helper_stmt: Stmt = parse_quote! {
                                const fn #helper_name<'b>() -> usize where &'b (): Sized { 3 }
                            };
                            func.block.stmts.insert(0, helper_stmt);
                            let array_stmt: Stmt = parse_quote! {
                                let _: [u8; #helper_name::<'a>()];
                            };
                            func.block.stmts.insert(1, array_stmt);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a const generic parameter that depends on a lifetime parameter via a const function within a nested or local context. It identifies functions with lifetime parameters, adds the necessary feature attributes if missing, inserts a const helper function with a lifetime-dependent where clause, and uses it in an array type within the function body. This transformation forces the compiler to handle lifetime-dependent const expressions in array lengths, potentially triggering region conversion errors during borrow checking of const generics."
    }
}