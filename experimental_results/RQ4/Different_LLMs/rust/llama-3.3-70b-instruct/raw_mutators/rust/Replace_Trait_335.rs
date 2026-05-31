use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Trait_335;

impl Mutator for Replace_Trait_335 {
    fn name(&self) -> &str {
        "Replace_Trait_335"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut traits: Vec<Ident> = Vec::new();

        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                traits.push(trait_item.ident.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::TraitObject(trait_object) = &*pat_type.ty {
                            if let Some(dyn_token) = trait_object.dyn_token {
                                if let Some(replacement_trait) = traits.choose(&mut thread_rng()) {
                                    let new_trait_path = SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: replacement_trait.clone(),
                                            arguments: PathArguments::None,
                                        }]),
                                    };

                                    let new_trait_bound = TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: new_trait_path,
                                    };

                                    let new_bounds = Punctuated::from_iter(vec![TypeParamBound::Trait(
                                        new_trait_bound,
                                    )]);

                                    let new_trait_object = TypeTraitObject {
                                        dyn_token: Some(dyn_token.clone()),
                                        bounds: new_bounds,
                                    };

                                    pat_type.ty = Box::new(Type::TraitObject(new_trait_object));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait objects in function parameters with other traits in the same scope. This transformation tests the compiler's handling of trait objects and their interactions, potentially revealing issues in trait resolution and object safety."
    }
}