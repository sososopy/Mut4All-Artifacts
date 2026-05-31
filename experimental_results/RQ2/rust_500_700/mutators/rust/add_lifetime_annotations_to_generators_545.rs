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

pub struct Add_Lifetime_Annotations_To_Generators_545;

impl Mutator for Add_Lifetime_Annotations_To_Generators_545 {
    fn name(&self) -> &str {
        "Add_Lifetime_Annotations_To_Generators_545"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut found_ref = false;
                        let mut new_bounds: Punctuated<TypeParamBound, Plus> = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let mut new_trait_bound = trait_bound.clone();
                                for segment in &trait_bound.path.segments {
                                    if segment.ident == "Iterator" {
                                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                            for arg in &args.args {
                                                if let GenericArgument::Type(Type::Reference(type_ref)) = arg {
                                                    found_ref = true;
                                                    let new_lifetime: Lifetime = parse_quote!('c);
                                                    let mut new_generics = item_fn.sig.generics.clone();
                                                    new_generics.params.push(GenericParam::Lifetime(LifetimeParam::new(new_lifetime.clone())));
                                                    item_fn.sig.generics = new_generics;

                                                    if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                                                        where_clause.predicates.push(parse_quote!('b: 'c));
                                                    } else {
                                                        item_fn.sig.generics.where_clause = Some(parse_quote!(where 'b: 'c));
                                                    }

                                                    let mut new_type_ref = type_ref.clone();
                                                    new_type_ref.lifetime = Some(new_lifetime);
                                                    let new_arg = GenericArgument::Type(Type::Reference(new_type_ref));
                                                    let new_args: Punctuated<_, Comma> = args.args.iter().map(|a| if let GenericArgument::Type(Type::Reference(_)) = a { new_arg.clone() } else { a.clone() }).collect();
                                                    let new_segment = PathSegment {
                                                        ident: segment.ident.clone(),
                                                        arguments: PathArguments::AngleBracketed(parse_quote!(<#new_args>)),
                                                    };
                                                    let new_path = SynPath {
                                                        leading_colon: trait_bound.path.leading_colon,
                                                        segments: trait_bound.path.segments.iter().map(|s| if s.ident == "Iterator" { new_segment.clone() } else { s.clone() }).collect(),
                                                    };
                                                    new_trait_bound.path = new_path;
                                                }
                                            }
                                        }
                                    }
                                }
                                new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        }
                        if found_ref {
                            type_impl_trait.bounds = new_bounds;
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