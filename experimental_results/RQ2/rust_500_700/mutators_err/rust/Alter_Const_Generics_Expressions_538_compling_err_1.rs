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

pub struct Alter_Const_Generics_Expressions_538;

impl Mutator for Alter_Const_Generics_Expressions_538 {
    fn name(&self) -> &str {
        "Alter_Const_Generics_Expressions_538"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some((_, generics, _)) = &item_struct.generics.split_for_impl() {
                    for param in &generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Some(default) = &const_param.default {
                                if let Expr::Binary(expr_binary) = &**default {
                                    let new_expr: Expr = match expr_binary.op {
                                        syn::BinOp::Add(_) => parse_quote! { #expr_binary.left * 2 },
                                        syn::BinOp::Mul(_) => parse_quote! { #expr_binary.left + 3 },
                                        _ => continue,
                                    };
                                    const_param.default = Some(Box::new(new_expr));
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