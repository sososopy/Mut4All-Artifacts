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
    TraitItem, TraitItemConst,
};

use crate::mutator::Mutator;

pub struct Alter_Associated_Constants_Usage_502;

impl Mutator for Alter_Associated_Constants_Usage_502 {
    fn name(&self) -> &str {
        "Alter_Associated_Constants_Usage_502"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                let has_associated_const = item_trait.items.iter().any(|trait_item| {
                    if let TraitItem::Const(_) = trait_item {
                        true
                    } else {
                        false
                    }
                });

                if has_associated_const {
                    for trait_item in &mut item_trait.items {
                        if let TraitItem::Method(method) = trait_item {
                            if method.default.is_some() {
                                let associated_const_usage = method.default.as_ref().unwrap().stmts.iter().any(|stmt| {
                                    if let Stmt::Expr(Expr::Path(ExprPath { path, .. }), _) = stmt {
                                        path.segments.iter().any(|seg| seg.ident == "VALUE")
                                    } else {
                                        false
                                    }
                                });

                                if associated_const_usage {
                                    method.default.as_mut().unwrap().stmts = vec![
                                        parse_quote! {
                                            let result = Self::VALUE + 10;
                                        },
                                        parse_quote! {
                                            result
                                        },
                                    ];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}