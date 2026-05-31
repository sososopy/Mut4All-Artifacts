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

pub struct Change_Struct_Trait_Bound_53;

impl Mutator for Change_Struct_Trait_Bound_53 {
    fn name(&self) -> &str {
        "Change_Struct_Trait_Bound_53"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics.params.first_mut() {
                    if let GenericParam::Type(type_param) = generics {
                        if let Some(bound) = type_param.bounds.first_mut() {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.path.segments.last_mut().unwrap().ident = Ident::new("Clone", Span::call_site());
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with type parameters having trait bounds. It changes the trait bound to an incompatible trait, such as from `Copy` to `Clone`. This transformation is intended to test the compiler's handling of trait bound mismatches, potentially leading to compilation errors if the types used do not satisfy the new trait bound."
    }
}