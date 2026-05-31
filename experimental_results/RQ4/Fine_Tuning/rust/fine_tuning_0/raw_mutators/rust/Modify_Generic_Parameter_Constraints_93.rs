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

pub struct Modify_Generic_Parameter_Constraints_93;

impl Mutator for Modify_Generic_Parameter_Constraints_93 {
    fn name(&self) -> &str {
        "Modify_Generic_Parameter_Constraints_93"
    }
    fn mutate(&self, file: &mut syn::File) {
        let additional_trait = Ident::new("Display", Span::call_site());
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    if let Some(generics) = &mut item_struct.generics.params.first_mut() {
                        if let GenericParam::Type(type_param) = generics {
                            if let Some(bounds) = &mut type_param.bounds.first_mut() {
                                if let TypeParamBound::Trait(trait_bound) = bounds {
                                    trait_bound.path.segments.push(PathSegment {
                                        ident: additional_trait.clone(),
                                        arguments: PathArguments::None,
                                    });
                                }
                            }
                        }
                    }
                }
                Item::Trait(item_trait) => {
                    if let Some(generics) = &mut item_trait.generics.params.first_mut() {
                        if let GenericParam::Type(type_param) = generics {
                            if let Some(bounds) = &mut type_param.bounds.first_mut() {
                                if let TypeParamBound::Trait(trait_bound) = bounds {
                                    trait_bound.path.segments.push(PathSegment {
                                        ident: additional_trait.clone(),
                                        arguments: PathArguments::None,
                                    });
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets generic parameters in struct and trait declarations that have existing constraints. It adds an additional trait constraint, specifically 'Display', to these parameters. This transformation tests the compiler's ability to handle multiple trait bounds and may uncover issues in trait resolution or implementation requirements."
    }
}