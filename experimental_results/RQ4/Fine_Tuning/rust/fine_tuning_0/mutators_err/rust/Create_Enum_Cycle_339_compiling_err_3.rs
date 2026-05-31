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

pub struct Create_Enum_Cycle_339;

impl Mutator for Create_Enum_Cycle_339 {
    fn name(&self) -> &str {
        "Create_Enum_Cycle_339"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                if let Some((_, discriminant)) = item_enum.variants.iter_mut().find_map(|variant| {
                    if let Some((_, expr)) = &variant.discriminant {
                        if let Expr::Binary(expr_binary) = expr {
                            if let Expr::Path(expr_path) = &*expr_binary.left {
                                if expr_path.path.segments.first().map_or(false, |seg| seg.ident == item_enum.ident) {
                                    return Some((variant, expr_binary));
                                }
                            }
                        }
                    }
                    None
                }) {
                    let new_expr: Expr = parse_quote! { #discriminant.right - #discriminant.left };
                    *discriminant = syn::ExprBinary {
                        attrs: vec![],
                        left: Box::new(new_expr),
                        op: syn::BinOp::Sub(token::Sub { spans: [Span::call_site()] }),
                        right: discriminant.right.clone(),
                    };
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enums with constant expressions involving their own variants. It identifies arithmetic expressions and introduces a cyclic reference by altering the expression to create potential compile-time evaluation issues or undefined behavior."
    }
}