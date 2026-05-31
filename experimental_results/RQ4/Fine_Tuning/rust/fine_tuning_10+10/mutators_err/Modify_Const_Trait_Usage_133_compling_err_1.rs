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

pub struct Modify_Const_Trait_Usage_133;

impl Mutator for Modify_Const_Trait_Usage_133 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Usage_133"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.attrs.iter().any(|attr| attr.path.is_ident("const_trait")) {
                    trait_item.attrs.retain(|attr| !attr.path.is_ident("const_trait"));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait declarations that use the #[const_trait] attribute. The mutation involves removing this attribute from the trait declaration. The rationale is to observe how the absence of the #[const_trait] attribute affects the usage of the trait in contexts where const trait implementations are expected, such as within const functions or with ~const bounds in type definitions. This can potentially reveal issues or bugs related to const trait implementations."
    }
}