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

pub struct Modify_Type_Alias_With_Non_Implementing_Type_232;

impl Mutator for Modify_Type_Alias_With_Non_Implementing_Type_232 {
    fn name(&self) -> &str {
        "Modify_Type_Alias_With_Non_Implementing_Type_232"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = vec![];

        for item in &mut file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(_) = *type_item.ty {
                    type_aliases.push(type_item.ident.clone());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                if let Type::Path(TypePath { path, .. }) = &*static_item.ty {
                    if type_aliases.contains(&path.segments.last().unwrap().ident) {
                        static_item.expr = Box::new(parse_quote! { "non_implementing_type" });
                    }
                }
            }
        }

        if type_aliases.is_empty() {
            if let Some(first_item) = file.items.first_mut() {
                let new_static: Item = parse_quote! {
                    static NON_IMPL: impl std::fmt::Debug = "non_implementing_type";
                };
                file.items.insert(0, new_static);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies type aliases using `impl Trait` and modifies assignments to these aliases with a type that does not implement the required trait. If no such assignment exists, it introduces a new static assignment."
    }
}