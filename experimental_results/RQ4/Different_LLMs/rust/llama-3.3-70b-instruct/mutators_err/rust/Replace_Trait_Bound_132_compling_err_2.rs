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

pub struct Replace_Trait_Bound_132;

impl Mutator for Replace_Trait_Bound_132 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_132"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.supertraits {
                    if let Type::Path(type_path) = trait_item {
                        let new_trait_name = Ident::new(&format!("New_{}", type_path.path.segments.last().unwrap().ident), type_path.path.segments.last().unwrap().ident.span());
                        let new_trait_bound = TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: new_trait_name,
                                    arguments: PathArguments::None,
                                }]),
                            },
                        };
                        *trait_item = Type::Path(TypePath {
                            qself: None,
                            path: new_trait_bound.path,
                        });
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    for bound in &mut predicate_type.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            let new_trait_name = Ident::new(&format!("New_{}", trait_bound.path.segments.last().unwrap().ident), trait_bound.path.segments.last().unwrap().ident.span());
                                            let new_trait_bound = TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: new_trait_name,
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            };
                                            *trait_bound = new_trait_bound;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds with new trait bounds that have similar names but are different. This transformation aims to test the compiler's ability to handle trait bounds and resolve trait implementations under altered type expectations."
    }
}