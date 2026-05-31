use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct Replace_Const_With_Static_75;

impl Mutator for Replace_Const_With_Static_75 {
    fn name(&self) -> &str {
        "Replace_Const_With_Static_75"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                let static_token = token::Static {
                    span: const_item.const_token.span,
                };
                let mut new_item = Item::Const(ItemConst {
                    attrs: const_item.attrs.clone(),
                    vis: const_item.vis.clone(),
                    static_token: Some(static_token),
                    mutability: const_item.mutability.clone(),
                    ident: const_item.ident.clone(),
                    colon_token: const_item.colon_token.clone(),
                    ty: const_item.ty.clone(),
                    eq_token: const_item.eq_token.clone(),
                    expr: const_item.expr.clone(),
                    semi_token: const_item.semi_token.clone(),
                });
                *item = new_item;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant declarations with static variables. This transformation tests the compiler's handling of constant evaluation and static variables, potentially leading to differences in behavior due to the distinct semantics of constants and statics."
    }
}