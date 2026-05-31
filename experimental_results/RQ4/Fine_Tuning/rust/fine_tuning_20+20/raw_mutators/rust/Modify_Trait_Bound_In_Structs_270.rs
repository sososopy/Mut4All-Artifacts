use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Modify_Trait_Bound_In_Structs_270;

impl Mutator for Modify_Trait_Bound_In_Structs_270 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_In_Structs_270"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_trait_bound = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if !type_param.bounds.is_empty() {
                            has_trait_bound = true;
                            break;
                        }
                    }
                }
                if !has_trait_bound {
                    continue;
                }
                let mut rng = thread_rng();
                let choice = rng.gen_range(0..3);
                match choice {
                    0 => {
                        for param in &mut item_struct.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                if !type_param.bounds.is_empty() {
                                    type_param.bounds = {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: SynPath::from(Ident::new(
                                                "Debug",
                                                Span::call_site(),
                                            )),
                                        }));
                                        bounds
                                    };
                                }
                            }
                        }
                    }
                    1 => {
                        for param in &mut item_struct.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                if !type_param.bounds.is_empty() {
                                    type_param.bounds.clear();
                                }
                            }
                        }
                    }
                    2 => {
                        for param in &mut item_struct.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                if !type_param.bounds.is_empty() {
                                    type_param.bounds.push(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath::from(Ident::new(
                                            "PartialEq",
                                            Span::call_site(),
                                        )),
                                    }));
                                }
                            }
                        }
                    }
                    _ => {}
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with generic parameters that have trait bounds. It randomly chooses to either change the trait bound to `Debug`, remove the trait bound entirely, or add an additional `PartialEq` trait bound. This transformation explores the compiler's handling of trait-bound generics by altering or removing constraints, potentially revealing issues with trait resolution, type inference, or code generation."
    }
}