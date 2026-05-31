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

pub struct Modify_Enum_Variant_With_PhantomData_180;

impl Mutator for Modify_Enum_Variant_With_PhantomData_180 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_With_PhantomData_180"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                for variant in &mut item_enum.variants {
                    if let Fields::Unnamed(fields_unnamed) = &mut variant.fields {
                        if !fields_unnamed.unnamed.is_empty() {
                            fields_unnamed.unnamed = Punctuated::new();
                            fields_unnamed.unnamed.push(parse_quote!(std::marker::PhantomData<i32>));
                        }
                    } else if let Fields::Named(fields_named) = &mut variant.fields {
                        fields_named.named.push(parse_quote!(phantom: std::marker::PhantomData<i32>));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}