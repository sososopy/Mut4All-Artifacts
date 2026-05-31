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

pub struct Modify_Type_Alias_With_Trait_47;

impl Mutator for Modify_Type_Alias_With_Trait_47 {
    fn name(&self) -> &str {
        "Modify_Type_Alias_With_Trait_47"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(type_impl_trait) = &*type_item.ty {
                    let mut traits = vec!["Clone", "Debug", "PartialEq", "PartialOrd"];
                    if let Some(first_bound) = type_impl_trait.bounds.first() {
                        if let TypeParamBound::Trait(trait_bound) = first_bound {
                            let current_trait = trait_bound.path.segments.last().unwrap().ident.to_string();
                            traits.retain(|&x| x != current_trait);
                        }
                    }
                    if let Some(new_trait) = traits.choose(&mut thread_rng()) {
                        type_item.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                            impl_token: token::Impl {
                                span: Span::call_site(),
                            },
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(TypeParamBound::Trait(TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(PathSegment {
                                                ident: Ident::new(new_trait, Span::call_site()),
                                                arguments: PathArguments::None,
                                            });
                                            segments
                                        },
                                    },
                                }));
                                bounds
                            },
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets type aliases using `impl Trait` syntax. It randomly replaces the trait being implemented with another compatible trait from a predefined list. This tests the compiler's ability to handle different trait obligations and ensures that trait unification and projection are correctly processed, potentially exposing issues in trait resolution."
    }
}