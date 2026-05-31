use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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
    Attribute, Meta, MetaList, NestedMeta,
};

use crate::mutator::Mutator;

pub struct Enum_With_Default_Const_Generics_Modification_267;

impl Mutator for Enum_With_Default_Const_Generics_Modification_267 {
    fn name(&self) -> &str {
        "Enum_With_Default_Const_Generics_Modification_267"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if let Some(repr_attr) = item_enum.attrs.iter().find(|attr| attr.path().is_ident("repr")) {
                    if let Ok(meta) = repr_attr.parse_meta() {
                        if let Meta::List(meta_list) = meta {
                            if meta_list.tokens.to_string().contains("u8") {
                                if let Some(default_const_generic) = item_enum.generics.params.iter_mut().find_map(|param| {
                                    if let GenericParam::Const(const_param) = param {
                                        if let Some(default) = &const_param.default {
                                            return Some((const_param, default.clone()));
                                        }
                                    }
                                    None
                                }) {
                                    let (const_param, default) = default_const_generic;
                                    let mut variant_values = Vec::new();
                                    for variant in &item_enum.variants {
                                        if let Some((_, Expr::Lit(ExprLit { lit: Lit::Int(lit_int), .. }))) = &variant.discriminant {
                                            if let Ok(value) = lit_int.base10_parse::<u8>() {
                                                variant_values.push(value);
                                            }
                                        }
                                    }
                                    if variant_values.len() >= 2 {
                                        let new_default_expr = Expr::Binary(ExprBinary {
                                            attrs: Vec::new(),
                                            left: Box::new(Expr::Lit(ExprLit {
                                                attrs: Vec::new(),
                                                lit: Lit::Int(LitInt::new(&variant_values[0].to_string(), Span::call_site())),
                                            })),
                                            op: BinOp::Add(Default::default()),
                                            right: Box::new(Expr::Binary(ExprBinary {
                                                attrs: Vec::new(),
                                                left: Box::new(Expr::Lit(ExprLit {
                                                    attrs: Vec::new(),
                                                    lit: Lit::Int(LitInt::new(&variant_values[1].to_string(), Span::call_site())),
                                                })),
                                                op: BinOp::Add(Default::default()),
                                                right: Box::new(Expr::Lit(ExprLit {
                                                    attrs: Vec::new(),
                                                    lit: Lit::Int(LitInt::new("250", Span::call_site())),
                                                })),
                                            })),
                                        });
                                        const_param.default = Some(new_default_expr);
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
        "The mutation operator targets enums with default const generics. It identifies enums with a #[repr(u8)] attribute and modifies the default const generic value by altering the arithmetic expression to exceed the u8 range. This involves using the values of existing enum variants and adding a large constant to ensure overflow, potentially exposing compiler bugs related to const generic range violations."
    }
}