use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Generic_Assoc_Type_With_Primitive_Type_81;

impl Mutator for Replace_Generic_Assoc_Type_With_Primitive_Type_81 {
    fn name(&self) -> &str {
        "Replace_Generic_Assoc_Type_With_Primitive_Type_81"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for assoc_type in &mut trait_item.generics.type_params() {
                    let primitive_type = match thread_rng().gen_range(0..3) {
                        0 => parse_quote!(i32),
                        1 => parse_quote!(bool),
                        2 => parse_quote!(char),
                        _ => unreachable!(),
                    };
                    assoc_type.default = Some(primitive_type);
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(assoc_type) = impl_item {
                        let primitive_type = match thread_rng().gen_range(0..3) {
                            0 => parse_quote!(i32),
                            1 => parse_quote!(bool),
                            2 => parse_quote!(char),
                            _ => unreachable!(),
                        };
                        *assoc_type.ty = primitive_type;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic associated types in trait definitions and implementations with primitive types (i32, bool, char). This transformation tests the compiler's handling of const generics and associated types, potentially uncovering bugs related to type resolution and inference."
    }
}