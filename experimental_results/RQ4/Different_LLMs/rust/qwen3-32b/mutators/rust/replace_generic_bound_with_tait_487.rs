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

pub struct Replace_Generic_Bound_With_Tait_487;

impl Mutator for Replace_Generic_Bound_With_Tait_487 {
    fn name(&self) -> &str {
        "Replace_Generic_Bound_With_Tait_487"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Check if Tait is already present
        let has_tait = file.items.iter().any(|item| {
            if let Item::Type(ty) = item {
                ty.ident == "Tait"
            } else {
                false
            }
        });
        if !has_tait {
            let tait_type = parse_quote! {
                type Tait = impl std::ops::Fn();
            };
            file.items.insert(0, Item::Type(tait_type));
        }

        // Process functions and methods
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                process_function(&mut func.sig.generics);
            } else if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        process_function(&mut method.sig.generics);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn process_function(generics: &mut syn::Generics) {
    // Process type parameters in generics
    for param in &mut generics.params {
        if let GenericParam::Type(type_param) = param {
            for bound in &mut type_param.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    let path = &mut trait_bound.path;
                    if let Some(last_segment) = path.segments.last_mut() {
                        let ident = last_segment.ident.to_string();
                        if ident == "FnOnce" || ident == "FnMut" || ident == "Fn" {
                            // Check if path arguments are empty
                            if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                                if args.args.is_empty() {
                                    // Add Tait as argument
                                    let tait_type = parse_quote!(Tait);
                                    let new_args = Punctuated::from_iter(vec![GenericArgument::Type(tait_type)]);
                                    last_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt::default(),
                                        args: new_args,
                                        gt_token: token::Gt::default(),
                                    });
                                }
                            } else if last_segment.arguments.is_empty() {
                                // No arguments, add them
                                let tait_type = parse_quote!(Tait);
                                let new_args = Punctuated::from_iter(vec![GenericArgument::Type(tait_type)]);
                                last_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: new_args,
                                    gt_token: token::Gt::default(),
                                });
                            }
                        }
                    }
                }
            }
        }
    }

    // Process where clause
    if let Some(where_clause) = &mut generics.where_clause {
        for predicate in &mut where_clause.predicates {
            if let syn::WherePredicate::Type(pred) = predicate {
                for bound in &mut pred.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        let path = &mut trait_bound.path;
                        if let Some(last_segment) = path.segments.last_mut() {
                            let ident = last_segment.ident.to_string();
                            if ident == "FnOnce" || ident == "FnMut" || ident == "Fn" {
                                // Check if path arguments are empty
                                if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                                    if args.args.is_empty() {
                                        // Add Tait as argument
                                        let tait_type = parse_quote!(Tait);
                                        let new_args = Punctuated::from_iter(vec![GenericArgument::Type(tait_type)]);
                                        last_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: new_args,
                                            gt_token: token::Gt::default(),
                                        });
                                    }
                                } else if last_segment.arguments.is_empty() {
                                    // No arguments, add them
                                    let tait_type = parse_quote!(Tait);
                                    let new_args = Punctuated::from_iter(vec![GenericArgument::Type(tait_type)]);
                                    last_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt::default(),
                                        args: new_args,
                                        gt_token: token::Gt::default(),
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