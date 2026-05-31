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

pub struct Swap_Field_Order_181;

impl Mutator for Swap_Field_Order_181 {
    fn name(&self) -> &str {
        "Swap_Field_Order_181"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if struct_item.fields.len() >= 2 {
                    let mut rng = thread_rng();
                    let fields: Vec<_> = struct_item.fields.iter().collect();
                    let idx1 = rng.gen_range(0..fields.len());
                    let idx2 = rng.gen_range(0..fields.len());
                    if idx1 != idx2 {
                        struct_item.fields.swap(idx1, idx2);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of two fields in a struct definition. This transformation tests the compiler's handling of struct layouts and field ordering, potentially triggering bugs in the layout and field system."
    }
}