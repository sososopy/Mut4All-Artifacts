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

pub struct Replace_Assoc_Type_With_Nested_Binder_413;

impl Mutator for Replace_Assoc_Type_With_Nested_Binder_413 {
    fn name(&self) -> &str {
        "Replace_Assoc_Type_With_Nested_Binder_413"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Bounded(bounded) = *pat_type.ty {
                            if let Some(trait_bound) = bounded.bounds.iter().find_map(|bound| {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    Some(trait_bound)
                                } else {
                                    None
                                }
                            }) {
                                if trait_bound.path.is_ident("Fn") {
                                    if let Some(bound_lifetimes) = &bounded.lifetimes {
                                        if let Some(first_lifetime) = bound_lifetimes.lifetimes.first() {
                                            let lifetime_str = first_lifetime.lifetime.ident.to_string();
                                            let lifetime_span = first_lifetime.lifetime.ident.span();
                                            if let PathArguments::Parenthesized(parens) = &trait_bound.path.segments.last().unwrap().arguments {
                                                for arg in &mut parens.inputs {
                                                    if let Type::Path(type_path) = arg {
                                                        if type_path.path.segments.len() == 2 {
                                                            let first_segment = type_path.path.segments[0].clone();
                                                            let second_segment = type_path.path.segments[1].clone();
                                                            let new_path = {
                                                                let mut segments = Punctuated::new();
                                                                let qself_type = Type::Path(TypePath {
                                                                    qself: None,
                                                                    path: SynPath {
                                                                        leading_colon: None,
                                                                        segments: Punctuated::from_iter(vec![first_segment.clone()]),
                                                                    },
                                                                });
                                                                let qself_trait = SynPath {
                                                                    leading_colon: None,
                                                                    segments: Punctuated::from_iter(vec![second_segment.clone()]),
                                                                };
                                                                let qself = QSelf {
                                                                    ty: Box::new(qself_type),
                                                                    position: 0,
                                                                };
                                                                let assoc_segment = PathSegment {
                                                                    ident: second_segment.ident.clone(),
                                                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                                        colon2_token: None,
                                                                        lt_token: token::Lt::default(),
                                                                        args: Punctuated::new(),
                                                                        gt_token: token::Gt::default(),
                                                                    }),
                                                                    qself: Some(qself),
                                                                };
                                                                segments.push(assoc_segment);
                                                                let mut inner_segment = PathSegment {
                                                                    ident: Ident::new("Inner", Span::call_site()),
                                                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                                        colon2_token: None,
                                                                        lt_token: token::Lt::default(),
                                                                        args: Punctuated::from_iter(vec![GenericArgument::Lifetime(Lifetime::new(
                                                                            &lifetime_str,
                                                                            lifetime_span,
                                                                        ))]),
                                                                        gt_token: token::Gt::default(),
                                                                    }),
                                                                    qself: None,
                                                                };
                                                                segments.push(inner_segment);
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            };
                                                            *arg = Type::Path(TypePath {
                                                                qself: None,
                                                                path: new_path,
                                                            });
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets higher-ranked function parameters (e.g., for<'a> fn(...)) and replaces inherent associated types (T::Assoc) with a nested region reference under a binder that directly captures the bound lifetime. This transformation forces the compiler to resolve an associated type nested under a binder with a directly captured lifetime, increasing the likelihood of region conversion errors and ICEs by creating complex, invalid type relationships that stress the trait and lifetime resolution systems."
    }
}