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

pub struct Modify_Generic_Parameter_Constraints_93;

impl Mutator for Modify_Generic_Parameter_Constraints_93 {
    fn name(&self) -> &str {
        "Modify_Generic_Parameter_Constraints_93"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_traits: HashSet<String> = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                existing_traits.insert(item_trait.ident.to_string());
            }
        }
        let mut additional_trait = None;
        for item in &file.items {
            if let Item::Use(item_use) = item {
                if let UseTree::Path(use_path) = &item_use.tree {
                    if let UseTree::Name(use_name) = &*use_path.tree {
                        if existing_traits.contains(&use_name.ident.to_string()) {
                            additional_trait = Some(use_name.ident.clone());
                            break;
                        }
                    }
                }
            }
        }
        if additional_trait.is_none() {
            for item in &file.items {
                if let Item::Trait(item_trait) = item {
                    additional_trait = Some(item_trait.ident.clone());
                    break;
                }
            }
        }
        if additional_trait.is_none() {
            return;
        }
        let additional_trait = additional_trait.unwrap();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if type_param.ident == "OT" || type_param.ident == "COT" {
                            let mut new_bounds = Punctuated::new();
                            for bound in &type_param.bounds {
                                new_bounds.push(bound.clone());
                            }
                            new_bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath::from(additional_trait.clone()),
                            }));
                            type_param.bounds = new_bounds;
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.last().unwrap().ident == "Conster" {
                        for param in &mut item_impl.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                if type_param.ident == "OT" || type_param.ident == "COT" {
                                    let mut new_bounds = Punctuated::new();
                                    for bound in &type_param.bounds {
                                        new_bounds.push(bound.clone());
                                    }
                                    new_bounds.push(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath::from(additional_trait.clone()),
                                    }));
                                    type_param.bounds = new_bounds;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets the generic parameter constraints of the `Conster` struct and its corresponding impl block. It modifies the constraints on the `OT` and `COT` generic parameters by adding an additional trait bound. The mutator first identifies an existing trait in the seed program that is not currently used as a constraint for `OT` or `COT`. It then updates the generic parameter bounds to include this additional trait. This transformation leverages the complexity of trait constraints in Rust's type system, aiming to trigger edge cases in trait resolution and generic instantiation."
    }
}