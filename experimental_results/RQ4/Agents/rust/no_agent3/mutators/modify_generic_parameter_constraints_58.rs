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

pub struct Modify_Generic_Parameter_Constraints_58;

impl Mutator for Modify_Generic_Parameter_Constraints_58 {
    fn name(&self) -> &str {
        "Modify_Generic_Parameter_Constraints_58"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut additional_trait = None;

        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident != "Bytes" {
                    additional_trait = Some(item_trait.ident.clone());
                    break;
                }
            }
        }

        if let Some(additional_trait) = additional_trait {
            for item in &mut file.items {
                match item {
                    Item::Struct(item_struct) => {
                        for param in &mut item_struct.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                for bound in &mut type_param.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.is_ident("Bytes") {
                                            trait_bound.path.segments.push(parse_quote!(+ #additional_trait));
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Item::Trait(item_trait) => {
                        for param in &mut item_trait.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                for bound in &mut type_param.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.is_ident("Bytes") {
                                            trait_bound.path.segments.push(parse_quote!(+ #additional_trait));
                                        }
                                    }
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
        "This mutation operator modifies generic parameter constraints in Rust code by identifying generic parameters with existing constraints and introducing an additional trait constraint. The operator looks for traits within the seed program that are not currently implemented by the constrained parameter and adds them to the constraint list. This can lead to unexpected compiler behavior, especially around const-generics and associated constants, by altering the trait bounds in generic parameters."
    }
}