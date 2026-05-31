use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, Pat, PatType, Path as SynPath,
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

pub struct Enum_Default_Const_Generics_Overflow_80;

impl Mutator for Enum_Default_Const_Generics_Overflow_80 {
    fn name(&self) -> &str {
        "Enum_Default_Const_Generics_Overflow_80"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let has_repr_u8 = item_enum.attrs.iter().any(|attr| {
                    attr.path().is_ident("repr") && attr.parse_meta().map_or(false, |meta| {
                        if let Meta::List(MetaList { nested, .. }) = meta {
                            nested.iter().any(|nested_meta| {
                                if let syn::NestedMeta::Meta(Meta::Path(path)) = nested_meta {
                                    path.is_ident("u8")
                                } else {
                                    false
                                }
                            })
                        } else {
                            false
                        }
                    })
                });

                if has_repr_u8 {
                    if let Some(default_generic) = item_enum.generics.params.iter_mut().find_map(|param| {
                        if let GenericParam::Const(const_param) = param {
                            Some(const_param)
                        } else {
                            None
                        }
                    }) {
                        if let Some(default_expr) = &mut default_generic.default {
                            let enum_ident = &item_enum.ident;
                            *default_expr = parse_quote! {
                                { #enum_ident::First as u8 + #enum_ident::Second as u8 + 250 }
                            };
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with default const generics and a `#[repr(u8)]` attribute. It modifies the default value of the const generic to include an arithmetic operation involving enum variants, ensuring the result exceeds the maximum value for `u8`. This transformation tests the compiler's handling of arithmetic overflow in const generics."
    }
}