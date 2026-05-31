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

pub struct Modify_Trait_Bound_In_Structs_108;

impl Mutator for Modify_Trait_Bound_In_Structs_108 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_In_Structs_108"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                for param in &mut generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if !type_param.bounds.is_empty() {
                            let mut rng = thread_rng();
                            let choice = rng.gen_range(0..3);
                            match choice {
                                0 => {
                                    // Change the trait bound to a different trait
                                    type_param.bounds.clear();
                                    type_param.bounds.push(parse_quote!(Debug));
                                }
                                1 => {
                                    // Remove the trait bound altogether
                                    type_param.bounds.clear();
                                }
                                2 => {
                                    // Add an additional trait bound
                                    type_param.bounds.push(parse_quote!(PartialEq));
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with generic parameters that have trait bounds. It modifies the trait bounds by either changing them to a different trait, removing them, or adding an additional trait. This helps explore how different trait combinations or the absence of trait bounds affect the code."
    }
}