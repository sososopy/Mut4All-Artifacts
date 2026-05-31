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

pub struct Swap_Trait_Bound_Type_Parameters_273;

impl Mutator for Swap_Trait_Bound_Type_Parameters_273 {
    fn name(&self) -> &str {
        "Swap_Trait_Bound_Type_Parameters_273"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let impl_trait_path = match &item_impl.trait_ {
                    Some((_, path, _)) => path,
                    None => continue,
                };
                let impl_trait_ident = &impl_trait_path.segments.last().unwrap().ident;
                let generic_params = &item_impl.generics.params;
                let mut rng = thread_rng();
                if let Some(where_clause) = &mut item_impl.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                if let Some(last_segment) = type_path.path.segments.last() {
                                    if last_segment.ident == *impl_trait_ident {
                                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                            let mut type_args = args.args.iter().collect::<Vec<_>>();
                                            if type_args.len() >= 2 {
                                                let first_two = type_args[0..2].to_vec();
                                                if let (GenericArgument::Type(first), GenericArgument::Type(second)) = (&first_two[0], &first_two[1]) {
                                                    if first != second {
                                                        let mut args_vec = args.args.iter().cloned().collect::<Vec<_>>();
                                                        args_vec.swap(0, 1);
                                                        args.args = Punctuated::from_iter(args_vec);
                                                    } else {
                                                        let available_params = generic_params.iter()
                                                            .filter_map(|param| match param {
                                GenericParam::Type(type_param) => Some(&type_param.ident),
                                _ => None,
                            })
                                                            .collect::<Vec<_>>();
                                                        if !available_params.is_empty() {
                                                            let replacement = available_params.choose(&mut rng).unwrap();
                                                            args.args[0] = GenericArgument::Type(Type::Path(TypePath {
                                qself: None,
                                path: SynPath::from(Ident::new(&replacement.to_string(), Spanned::span(replacement))),
                            }));
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
        "The mutation operator targets trait implementation blocks where the where clause contains a bound referencing the same trait being implemented. It identifies predicates of form Self: Trait<A, B> and swaps the first two type arguments if distinct, or replaces a repeated argument with another type parameter from the context. This alters trait resolution paths, potentially creating cyclic dependencies or mismatched obligations to trigger internal compiler errors."
    }
}