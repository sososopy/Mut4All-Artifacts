use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_With_Non_Const_402;

impl Mutator for Replace_Const_With_Non_Const_402 {
    fn name(&self) -> &str {
        "Replace_Const_With_Non_Const_402"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                let const_token = item_const.const_token;
                let ident = item_const.ident;
                let ty = item_const.ty;
                let expr = item_const.expr;
                let vis = item_const.vis;
                let attrs = item_const.attrs;

                let let_stmt = parse_quote! {
                    #vis let #ident: #ty = #expr;
                };

                *item = Item::Let(ItemLet {
                    attrs,
                    let_token: const_token,
                    pat: Pat::Ident(PatIdent {
                        attrs: vec![],
                        by_ref: None,
                        mutability: None,
                        ident,
                        subpat: None,
                    }),
                    ty: Some(ty),
                    init: Some(expr),
                    semi_token: token::Semi::default(),
                });
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Const(item_const) = impl_item {
                        let const_token = item_const.const_token;
                        let ident = item_const.ident;
                        let ty = item_const.ty;
                        let expr = item_const.expr;
                        let vis = item_const.vis;
                        let attrs = item_const.attrs;

                        let let_stmt = parse_quote! {
                            #vis let #ident: #ty = #expr;
                        };

                        *impl_item = ImplItem::Let(ItemLet {
                            attrs,
                            let_token: const_token,
                            pat: Pat::Ident(PatIdent {
                                attrs: vec![],
                                by_ref: None,
                                mutability: None,
                                ident,
                                subpat: None,
                            }),
                            ty: Some(ty),
                            init: Some(expr),
                            semi_token: token::Semi::default(),
                        });
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant values with non-constant values, potentially triggering bugs related to generic const expressions, associated const equality, and other constant contexts."
    }
}