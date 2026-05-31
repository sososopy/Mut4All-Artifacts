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

pub struct Replace_Static_With_Instance_Variable_62;

impl Mutator for Replace_Static_With_Instance_Variable_62 {
    fn name(&self) -> &str {
        "Replace_Static_With_Instance_Variable_62"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                let mut new_item = Item::Let(syn::ItemLet {
                    attrs: static_item.attrs.clone(),
                    let_token: token::Let {
                        span: static_item.vis.span(),
                    },
                    pat: Pat::Type(PatType {
                        attrs: static_item.attrs.clone(),
                        pat: static_item.expr.clone(),
                        colon_token: static_item.colon_token,
                        ty: static_item.ty.clone(),
                    }),
                    init: Some((token::Eq { span: static_item.eq_token.span }, static_item.expr.clone())),
                    semi_token: static_item.semi_token,
                });
                *item = new_item;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces static variables with instance variables to test the Rust compiler's handling of const evaluation and static initialization. By changing the scope and lifetime of variables, it can expose bugs related to A-const-eval and initialization order."
    }
}