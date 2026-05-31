use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Enum_With_Default_Const_Generics_Modification_80;

impl Mutator for Enum_With_Default_Const_Generics_Modification_80 {
    fn name(&self) -> &str {
        "Enum_With_Default_Const_Generics_Modification_80"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if item_enum
                    .attrs
                    .iter()
                    .any(|attr| attr.path().is_ident("repr"))
                    && item_enum
                        .generics
                        .params
                        .iter()
                        .any(|param| matches!(param, GenericParam::Const(_)))
                {
                    let repr_type = item_enum
                        .attrs
                        .iter()
                        .find_map(|attr| {
                            if attr.path().is_ident("repr") {
                                attr.parse_args::<Type>().ok()
                            } else {
                                None
                            }
                        })
                        .unwrap_or_else(|| parse_quote!(u8));
                    let max_value = match repr_type {
                        Type::Path(type_path) => {
                            if let Some(segment) = type_path.path.segments.last() {
                                match segment.ident.to_string().as_str() {
                                    "u8" => 255,
                                    "u16" => 65535,
                                    "u32" => 4294967295,
                                    "u64" => 18446744073709551615,
                                    "usize" => usize::MAX,
                                    _ => 255,
                                }
                            } else {
                                255
                            }
                        }
                        _ => 255,
                    };
                    if let Some(const_generic_param) = item_enum
                        .generics
                        .params
                        .iter_mut()
                        .find_map(|param| match param {
                            GenericParam::Const(const_param) => Some(const_param),
                            _ => None,
                        })
                    {
                        if let Some(default_expr) = &mut const_generic_param.default {
                            if let Expr::Block(expr_block) = &mut **default_expr {
                                let mut variant_values = Vec::new();
                                for variant in &item_enum.variants {
                                    if let Some((_, expr)) = &variant.discriminant {
                                        variant_values.push(expr.clone());
                                    } else {
                                        variant_values.push(parse_quote!(0));
                                    }
                                }
                                let sum_expr = variant_values.into_iter().fold(
                                    parse_quote!(0),
                                    |acc, expr| parse_quote!(#acc + #expr),
                                );
                                let overflow_expr =
                                    parse_quote!(#sum_expr + #max_value - 1);
                                expr_block.block.stmts =
                                    vec![Stmt::Expr(overflow_expr, None)];
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enums with const generics and a repr attribute, specifically those with a default const generic expression. It modifies the default expression to perform arithmetic operations on the enum variants' discriminants, ensuring the result exceeds the maximum value of the enum's underlying type. This transformation leverages const evaluation and overflow behavior to stress-test the compiler's handling of const generics and enum representations."
    }
}