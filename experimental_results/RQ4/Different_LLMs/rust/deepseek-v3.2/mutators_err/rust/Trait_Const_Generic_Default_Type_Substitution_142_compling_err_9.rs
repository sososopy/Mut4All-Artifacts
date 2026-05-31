use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, PathSegment, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, TypeTraitObject, parse_quote,
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

pub struct Trait_Const_Generic_Default_Type_Substitution_142;

impl Mutator for Trait_Const_Generic_Default_Type_Substitution_142 {
    fn name(&self) -> &str {
        "Trait_Const_Generic_Default_Type_Substitution_142"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                trait_names.insert(item_trait.ident.clone());
            }
        }
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for param in &mut item_trait.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            let mut candidate_traits = trait_names.clone();
                            candidate_traits.insert(item_trait.ident.clone());
                            let candidate_traits_vec: Vec<_> = candidate_traits.into_iter().collect();
                            let mut rng = thread_rng();
                            let selected_trait = candidate_traits_vec.choose(&mut rng);
                            if let Some(trait_ident) = selected_trait {
                                let new_type = if rng.gen_bool(0.5) {
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: trait_ident.clone(),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    })
                                } else {
                                    Type::TraitObject(TypeTraitObject {
                                        dyn_token: Some(token::Dyn::default()),
                                        bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: trait_ident.clone(),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        })]),
                                    })
                                };
                                const_param.ty = Box::new(new_type);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions with const generic parameters having default values. It substitutes the type annotation of such const parameters with a type referencing either the trait itself or another trait from the current scope, using either a direct path or a trait object. This creates potential cycles or invalid type contexts in const default expressions, aiming to trigger ICEs during HIR well-formedness checking."
    }
}