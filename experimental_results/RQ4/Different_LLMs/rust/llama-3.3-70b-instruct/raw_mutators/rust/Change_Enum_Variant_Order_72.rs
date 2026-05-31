use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Change_Enum_Variant_Order_72;

impl Mutator for Change_Enum_Variant_Order_72 {
    fn name(&self) -> &str {
        "Change_Enum_Variant_Order_72"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut variants: Vec<_> = enum_item.variants.clone();
                variants.shuffle(&mut thread_rng());
                enum_item.variants = variants;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the order of enum variants. This transformation tests the compiler's handling of enum layouts and may expose bugs related to transmutability and discriminant values."
    }
}