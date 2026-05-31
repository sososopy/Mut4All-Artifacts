use proc_macro2::{Span, TokenStream};
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
    Attribute, Meta, MetaList, parse::Parser,
};

use crate::mutator::Mutator;

pub struct Enum_With_Exotic_Discriminant_61;

impl Mutator for Enum_With_Exotic_Discriminant_61 {
    fn name(&self) -> &str {
        "Enum_With_Exotic_Discriminant_61"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                if let Some(attr) = item_enum.attrs.iter().find(|attr| attr.path().is_ident("repr")) {
                    if let Ok(meta_list) = attr.parse_args::<MetaList>() {
                        if meta_list.tokens.to_string().contains("usize") {
                            for variant in &mut item_enum.variants {
                                if let Some((_, expr)) = &mut variant.discriminant {
                                    *expr = parse_quote! { (-1isize) as usize };
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Const(item_const) = item {
                if let syn::Type::Path(type_path) = &*item_const.ty {
                    if type_path.path.segments.last().map_or(false, |segment| segment.ident == "MyEnum") {
                        if let syn::Expr::Unsafe(expr_unsafe) = &mut *item_const.expr {
                            if let syn::Stmt::Expr(syn::Expr::Call(expr_call), _) = &mut expr_unsafe.block.stmts[0] {
                                if let syn::Expr::Path(expr_path) = &*expr_call.func {
                                    if expr_path.path.is_ident("std::mem::transmute") {
                                        if let syn::Expr::Lit(expr_lit) = &mut expr_call.args[0] {
                                            if let syn::Lit::Int(lit_int) = &mut expr_lit.lit {
                                                *lit_int = syn::LitInt::new("4", lit_int.span());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with explicit discriminants, replacing one with a non-trivial expression. It also modifies constant usage of the enum to trigger compile-time evaluation issues. This transformation aims to test the compiler's handling of exotic discriminants and constant evaluation, potentially uncovering bugs in enum layout assumptions or value conversions."
    }
}