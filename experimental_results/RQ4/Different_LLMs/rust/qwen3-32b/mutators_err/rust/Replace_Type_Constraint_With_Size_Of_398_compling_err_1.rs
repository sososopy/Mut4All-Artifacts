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

pub struct Replace_Type_Constraint_With_Size_Of_398;

impl Mutator for Replace_Type_Constraint_With_Size_Of_398 {
    fn name(&self) -> &str {
        "Replace_Type_Constraint_With_Size_Of_398"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Check if IsTrue trait is present
        let has_is_true = file.items.iter().any(|item| {
            if let syn::Item::Trait(trait_item) = item {
                trait_item.ident == "IsTrue"
            } else {
                false
            }
        });

        if !has_is_true {
            let new_trait = parse_quote! {
                pub trait IsTrue<const N: bool> {}
            };
            file.items.insert(0, syn::Item::Trait(new_trait));
        }

        // Process each impl item
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some(generics) = &mut item_impl.generics {
                    if !generics.params.is_empty() {
                        // Find the first type parameter's ident
                        let first_type_param = generics.params.iter().find_map(|param| {
                            if let GenericParam::Type(type_param) = param {
                                Some(&type_param.ident)
                            } else {
                                None
                            }
                        });

                        if let Some(first_type_param) = first_type_param {
                            let where_clause = generics.where_clause.get_or_insert_with(|| {
                                syn::WhereClause {
                                    where_token: Default::default(),
                                    predicates: Punctuated::new(),
                                }
                            });

                            // Create the bounded_ty as ()
                            let bounded_ty = syn::Type::Tuple(syn::TypeTuple {
                                paren_token: Default::default(),
                                elems: Punctuated::new(),
                            });

                            // Create the expression using the first type parameter
                            let expr = parse_quote! { std::mem::size_of::<#first_type_param>() == 0 };

                            // Create the IsTrue path with const argument
                            let path_segment = syn::PathSegment {
                                ident: syn::Ident::new("IsTrue", Span::call_site()),
                                arguments: syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: {
                                        let const_arg = syn::GenericArgument::Const(syn::Expr::from(expr));
                                        Punctuated::from_iter(vec![const_arg])
                                    },
                                    gt_token: token::Gt::default(),
                                }),
                            };

                            let path = syn::Path {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![path_segment]),
                            };

                            let trait_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                                paren_token: None,
                                modifier: syn::TraitBoundModifier::None,
                                lifetimes: None,
                                path: path.clone(),
                            });

                            let mut bounds = Punctuated::new();
                            bounds.push(trait_bound);

                            let predicate = syn::WherePredicate::Type(syn::PredicateType {
                                lifetimes: None,
                                bounded_ty: Box::new(bounded_ty),
                                colon_token: Default::default(),
                                bounds,
                            });

                            // Clear existing predicates and add the new one
                            where_clause.predicates.clear();
                            where_clause.predicates.push(predicate);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}