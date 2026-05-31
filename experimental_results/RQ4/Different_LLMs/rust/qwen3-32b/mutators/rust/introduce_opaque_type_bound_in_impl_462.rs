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

pub struct Introduce_Opaque_Type_Bound_In_Impl_462;

impl Mutator for Introduce_Opaque_Type_Bound_In_Impl_462 {
    fn name(&self) -> &str {
        "Introduce_Opaque_Type_Bound_In_Impl_462"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut opaque_types = Vec::new();
        let mut traits = Vec::new();

        // First pass: collect opaque types and traits
        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(_) = &*item_type.ty {
                    opaque_types.push(item_type.ident.clone());
                }
            } else if let Item::Trait(item_trait) = item {
                traits.push(item_trait.ident.clone());
            }
        }

        // Second pass: process impls and add where clauses
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if !impl_item.generics.params.is_empty() && !traits.is_empty() && !opaque_types.is_empty() {
                    let mut rng = thread_rng();
                    let trait_name = traits.choose(&mut rng).unwrap();
                    let opaque_name = opaque_types.choose(&mut rng).unwrap();

                    let trait_segment = syn::PathSegment {
                        ident: trait_name.clone(),
                        arguments: syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                            colon2_token: None,
                            lt_token: token::Lt::default(),
                            args: {
                                let mut args = Punctuated::new();
                                args.push(syn::GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![syn::PathSegment {
                                            ident: opaque_name.clone(),
                                            arguments: syn::PathArguments::None,
                                        }]),
                                    },
                                })));
                                args
                            },
                            gt_token: token::Gt::default(),
                        }),
                    };

                    let trait_path = syn::Path {
                        leading_colon: None,
                        segments: Punctuated::from_iter(vec![trait_segment]),
                    };

                    let trait_bound = syn::TraitBound {
                        paren_token: None,
                        modifier: syn::TraitBoundModifier::None,
                        lifetimes: None,
                        path: trait_path,
                    };

                    let mut bounds = Punctuated::new();
                    bounds.push(syn::TypeParamBound::Trait(trait_bound));

                    let where_clause = impl_item.generics.where_clause.get_or_insert(syn::WhereClause {
                        where_token: token::Where::default(),
                        predicates: Punctuated::new(),
                    });

                    if let Some(generic_param) = impl_item.generics.params.first() {
                        if let syn::GenericParam::Type(type_param) = generic_param {
                            let bounded_ty = syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![syn::PathSegment {
                                        ident: type_param.ident.clone(),
                                        arguments: syn::PathArguments::None,
                                    }]),
                                },
                            });

                            let predicate = syn::WherePredicate::Type(syn::PredicateType {
                                lifetimes: None,
                                bounded_ty,
                                colon_token: token::Colon::default(),
                                bounds,
                            });

                            where_clause.predicates.push(predicate);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator introduces a complex opaque type bound in trait impls by modifying the where clause to include a reference to an existing opaque type alias and trait. This creates ambiguous type relationships that stress the compiler's layout computation and trait resolution systems, potentially exposing bugs in type inference and opaque type handling."
    }
}