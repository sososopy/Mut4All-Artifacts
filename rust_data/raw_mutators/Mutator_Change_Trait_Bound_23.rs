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

pub struct Mutator_Change_Trait_Bound_23;

impl Mutator for Mutator_Change_Trait_Bound_23 {
    fn name(&self) -> &str {
        "Mutator_Change_Trait_Bound_23"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics.params.first_mut() {
                    if let GenericParam::Type(type_param) = generics {
                        if let Some(TypeParamBound::Trait(trait_bound)) = type_param.bounds.first_mut() {
                            let new_trait: Path = parse_quote!(Clone);
                            trait_bound.path = new_trait;
                        }
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation targets struct definitions with type parameters having trait bounds. It changes the trait bound to an incompatible one, such as from Copy to Clone, to test the compiler's handling of trait mismatches."
    }
}