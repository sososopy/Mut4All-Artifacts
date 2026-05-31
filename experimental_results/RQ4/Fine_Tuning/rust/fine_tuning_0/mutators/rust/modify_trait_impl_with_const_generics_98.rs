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

pub struct Modify_Trait_Impl_With_Const_Generics_98;

impl Mutator for Modify_Trait_Impl_With_Const_Generics_98 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_With_Const_Generics_98"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(item_trait) = item {
                if item_trait.ident == "TraitName" {
                    item_trait.items.push(syn::TraitItem::Const(syn::TraitItemConst {
                        attrs: Vec::new(),
                        const_token: Default::default(),
                        ident: Ident::new("ASSOC", Span::call_site()),
                        colon_token: Default::default(),
                        ty: parse_quote!(usize),
                        default: None,
                        semi_token: Default::default(),
                        generics: syn::Generics::default(),
                    }));
                }
            } else if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.last().unwrap().ident == "TraitName" {
                        item_impl.generics.params.push(parse_quote!(const M: u64));
                        item_impl.items.push(syn::ImplItem::Const(syn::ImplItemConst {
                            attrs: Vec::new(),
                            vis: syn::Visibility::Inherited,
                            defaultness: None,
                            const_token: Default::default(),
                            ident: Ident::new("ASSOC", Span::call_site()),
                            colon_token: Default::default(),
                            ty: parse_quote!(usize),
                            expr: parse_quote!(M as usize),
                            semi_token: Default::default(),
                            eq_token: Default::default(),
                            generics: syn::Generics::default(),
                        }));
                    }
                }
            }
        }

        file.items.push(syn::Item::Fn(syn::ItemFn {
            attrs: Vec::new(),
            vis: syn::Visibility::Inherited,
            sig: syn::Signature {
                constness: None,
                asyncness: None,
                unsafety: None,
                abi: None,
                fn_token: Default::default(),
                ident: Ident::new("use_trait", Span::call_site()),
                generics: syn::Generics::default(),
                paren_token: Default::default(),
                inputs: Punctuated::new(),
                variadic: None,
                output: syn::ReturnType::Type(
                    Default::default(),
                    Box::new(parse_quote!([u8; <[u8; 5] as TraitName>::ASSOC])),
                ),
            },
            block: Box::new(syn::Block {
                brace_token: Default::default(),
                stmts: Vec::new(),
            }),
        }));
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}