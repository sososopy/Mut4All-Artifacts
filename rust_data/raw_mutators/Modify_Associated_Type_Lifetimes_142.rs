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

pub struct Modify_Associated_Type_Lifetimes_142;

impl Mutator for Modify_Associated_Type_Lifetimes_142 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_Lifetimes_142"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                let mut visitor = LifetimeModifier;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions using associated types with lifetime arguments. It identifies functions that take trait objects with associated types expecting specific numbers of lifetime parameters. The mutation alters the number of lifetime parameters provided to the associated type to create a mismatch error, potentially exposing compiler issues."
    }
}

struct LifetimeModifier;

impl VisitMut for LifetimeModifier {
    fn visit_type_path_mut(&mut self, type_path: &mut TypePath) {
        if let Some(last_segment) = type_path.path.segments.last_mut() {
            if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                let lifetime_count = args.args.iter().filter(|arg| matches!(arg, GenericArgument::Lifetime(_))).count();
                if lifetime_count > 0 {
                    args.args = Punctuated::new();
                } else {
                    args.args.push(GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site())));
                    args.args.push(GenericArgument::Lifetime(Lifetime::new("'b", Span::call_site())));
                }
            }
        }
        syn::visit_mut::visit_type_path_mut(self, type_path);
    }
}