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

pub struct Introduce_Non_Existent_Associated_Type_In_Const_Generic_Impl_391;

impl Mutator for Introduce_Non_Existent_Associated_Type_In_Const_Generic_Impl_391 {
    fn name(&self) -> &str {
        "Introduce_Non_Existent_Associated_Type_In_Const_Generic_Impl_391"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut has_const_generic = false;
                for param in &item_impl.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_generic = true;
                        break;
                    }
                }
                if !has_const_generic {
                    continue;
                }
                if let Some(where_clause) = &mut item_impl.generics.where_clause {
                    let predicates = &mut where_clause.predicates;
                    let mut indices: Vec<usize> = (0..predicates.len()).collect();
                    indices.shuffle(&mut rng);
                    for idx in indices {
                        if let WherePredicate::Type(pred_type) = &mut predicates[idx] {
                            if let Type::Path(type_path) = &pred_type.bounded_ty {
                                let mut trait_bounds = Vec::new();
                                for bound in &pred_type.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        trait_bounds.push(trait_bound);
                                    }
                                }
                                if !trait_bounds.is_empty() {
                                    let selected_bound = trait_bounds.choose(&mut rng).unwrap();
                                    let mut new_bounds = pred_type.bounds.clone();
                                    let mut new_predicate = pred_type.clone();
                                    let non_existent_assoc = Ident::new("NonExistentOutput", Span::call_site());
                                    let mut new_trait_bound = selected_bound.clone();
                                    let mut new_path = new_trait_bound.path.clone();
                                    if let Some(last_seg) = new_path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(args) = &mut last_seg.arguments {
                                            args.args.push(GenericArgument::AssocType(AssocType {
                                                ident: non_existent_assoc.clone(),
                                                generics: None,
                                                eq_token: Default::default(),
                                                ty: Type::Path(type_path.clone()),
                                            }));
                                        } else {
                                            last_seg.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: Default::default(),
                                                args: {
                                                    let mut punctuated = Punctuated::new();
                                                    punctuated.push(GenericArgument::AssocType(AssocType {
                                                        ident: non_existent_assoc.clone(),
                                                        generics: None,
                                                        eq_token: Default::default(),
                                                        ty: Type::Path(type_path.clone()),
                                                    }));
                                                    punctuated
                                                },
                                                gt_token: Default::default(),
                                            });
                                        }
                                    }
                                    new_trait_bound.path = new_path;
                                    new_bounds.push(TypeParamBound::Trait(new_trait_bound.clone()));
                                    new_predicate.bounds = new_bounds;
                                    predicates[idx] = WherePredicate::Type(new_predicate);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets impl blocks that use generic const expressions. It modifies a trait bound within the where clause to reference a non-existent associated type (NonExistentOutput). This creates a scenario where the compiler must resolve a malformed associated type projection during const evaluation in a generic context, potentially triggering bugs in the compiler's handling of associated types within const generic expressions."
    }
}