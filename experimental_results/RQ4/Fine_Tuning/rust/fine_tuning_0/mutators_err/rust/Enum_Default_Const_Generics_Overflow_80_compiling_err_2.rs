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
                if let Some(attr) = item_enum.attrs.iter().find(|attr| attr.path().is_ident("repr")) {
                    if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                        if meta_list.tokens.to_string().contains("u8") {
                            if let Some(default_generic) = item_enum.generics.params.iter_mut().find_map(|param| {
                                if let GenericParam::Const(const_param) = param {
                                    Some(const_param)
                                } else {
                                    None
                                }
                            }) {
                                if let Some(default_expr) = &mut default_generic.default {
                                    *default_expr = parse_quote! {
                                        { #item_enum::First as u8 + #item_enum::Second as u8 + 250 }
                                    };
                                }
                            }
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