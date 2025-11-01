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
};

use crate::mutator::Mutator;

pub struct Enum_With_Exotic_Discriminant_31;

impl Mutator for Enum_With_Exotic_Discriminant_31 {
    fn name(&self) -> &str {
        "Enum_With_Exotic_Discriminant_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if let Some(attr) = item_enum.attrs.iter().find(|attr| attr.path.is_ident("repr")) {
                    if let Ok(meta) = attr.parse_meta() {
                        if let Meta::List(meta_list) = meta {
                            if meta_list.nested.iter().any(|nested| match nested {
                                NestedMeta::Meta(Meta::Path(path)) => path.is_ident("usize"),
                                _ => false,
                            }) {
                                for variant in &mut item_enum.variants {
                                    if let Some((_, Expr::Lit(expr_lit))) = &variant.discriminant {
                                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                            let original_value: i128 = lit_int.base10_parse().unwrap_or(0);
                                            let mutated_value = if original_value > 0 {
                                                -1isize as i128
                                            } else {
                                                original_value.wrapping_add(1)
                                            };
                                            variant.discriminant = Some((
                                                Default::default(),
                                                Expr::Cast(ExprCast {
                                                    attrs: vec![],
                                                    expr: Box::new(Expr::Lit(ExprLit {
                                                        attrs: vec![],
                                                        lit: syn::Lit::Int(syn::LitInt::new(
                                                            &mutated_value.to_string(),
                                                            lit_int.span(),
                                                        )),
                                                    })),
                                                    as_token: Default::default(),
                                                    ty: Box::new(Type::Path(TypePath {
                                                        qself: None,
                                                        path: syn::Path::from(Ident::new("usize", Span::call_site())),
                                                    })),
                                                }),
                                            ));
                                            break;
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
        "This mutation operator targets enum definitions with explicit discriminants, specifically those with a `repr(usize)` attribute. It identifies such enums and mutates one of their discriminant values to a non-trivial expression, such as a negative number cast to `usize`. This aims to test the robustness of the code by introducing potential out-of-bounds or invalid values during compile-time evaluation."
    }
}