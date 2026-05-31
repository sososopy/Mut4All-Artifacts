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

pub struct TAIT_Trait_Bound_Permutation_471;

impl Mutator for TAIT_Trait_Bound_Permutation_471 {
    fn name(&self) -> &str {
        "TAIT_Trait_Bound_Permutation_471"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = TAITVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with bounds containing type alias impl trait (TAIT) with generic parameters. It permutes the generic arguments within the TAIT by swapping type parameters from the implementation's scope. This transformation alters trait resolution relationships, potentially exposing bugs in the compiler's handling of TAIT resolution, associated type projection, and monomorphization, especially when bounds become impossible to satisfy."
    }
}

struct TAITVisitor;

impl TAITVisitor {
    fn new() -> Self {
        TAITVisitor
    }
}

impl VisitMut for TAITVisitor {
    fn visit_item_impl_mut(&mut self, node: &mut syn::ItemImpl) {
        let generic_params: Vec<Ident> = node.generics.params.iter()
            .filter_map(|param| match param {
                GenericParam::Type(type_param) => Some(type_param.ident.clone()),
                _ => None,
            })
            .collect();
        
        if generic_params.len() < 2 {
            return;
        }

        let mut bounds_to_modify = Vec::new();
        
        if let Some(where_clause) = &node.generics.where_clause {
            for pred in where_clause.predicates.iter() {
                if let WherePredicate::Type(pred_type) = pred {
                    self.collect_tait_bounds(&pred_type.bounded_ty, &pred_type.bounds, &generic_params, &mut bounds_to_modify);
                }
            }
        }

        for (ty, bounds, tait_path, tait_args) in bounds_to_modify {
            if let Some(new_args) = self.permute_tait_args(&tait_args, &generic_params) {
                let new_tait = self.build_tait_type(&tait_path, &new_args);
                let new_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                    paren_token: None,
                    modifier: syn::TraitBoundModifier::None,
                    lifetimes: None,
                    path: syn::Path {
                        leading_colon: None,
                        segments: {
                            let mut segs = Punctuated::new();
                            segs.push(syn::PathSegment {
                                ident: new_tait,
                                arguments: PathArguments::None,
                            });
                            segs
                        },
                    },
                });
                if let Some(pos) = bounds.iter().position(|b| {
                    if let TypeParamBound::Trait(tb) = b {
                        tb.path.segments.last().map(|seg| seg.ident.to_string()) == Some(tait_path.to_string())
                    } else {
                        false
                    }
                }) {
                    let mut bounds_mut = bounds.clone();
                    bounds_mut.replace(pos, new_bound);
                    *bounds = bounds_mut;
                }
            }
        }

        syn::visit_mut::visit_item_impl_mut(self, node);
    }
}

impl TAITVisitor {
    fn collect_tait_bounds<'a>(&self, bounded_ty: &syn::Type, bounds: &'a Punctuated<syn::TypeParamBound, token::Plus>, generic_params: &[Ident], result: &mut Vec<(syn::Type, &'a Punctuated<syn::TypeParamBound, token::Plus>, Ident, Punctuated<syn::GenericArgument, token::Comma>)>) {
        for bound in bounds.iter() {
            if let TypeParamBound::Trait(trait_bound) = bound {
                for segment in &trait_bound.path.segments {
                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                        for arg in &args.args {
                            if let GenericArgument::Type(ty) = arg {
                                if let Some((tait_ident, tait_args)) = self.extract_tait(ty) {
                                    if self.contains_generic_params(&tait_args, generic_params) {
                                        result.push((bounded_ty.clone(), bounds, tait_ident.clone(), tait_args.clone()));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn extract_tait(&self, ty: &syn::Type) -> Option<(Ident, Punctuated<syn::GenericArgument, token::Comma>)> {
        if let Type::Path(type_path) = ty {
            if type_path.path.segments.len() == 1 {
                let segment = &type_path.path.segments[0];
                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                    return Some((segment.ident.clone(), args.args.clone()));
                }
            }
        }
        None
    }

    fn contains_generic_params(&self, args: &Punctuated<syn::GenericArgument, token::Comma>, generic_params: &[Ident]) -> bool {
        args.iter().any(|arg| {
            if let GenericArgument::Type(ty) = arg {
                if let Type::Path(type_path) = ty {
                    return generic_params.contains(&type_path.path.segments[0].ident);
                }
            }
            false
        })
    }

    fn permute_tait_args(&self, args: &Punctuated<syn::GenericArgument, token::Comma>, generic_params: &[Ident]) -> Option<Punctuated<syn::GenericArgument, token::Comma>> {
        let mut new_args = args.clone();
        let type_args: Vec<&GenericArgument> = args.iter().collect();
        
        if type_args.len() >= 2 {
            let mut rng = thread_rng();
            let mut indices: Vec<usize> = (0..type_args.len()).collect();
            indices.shuffle(&mut rng);
            
            new_args.clear();
            for &idx in &indices {
                new_args.push(type_args[idx].clone());
            }
            Some(new_args)
        } else if type_args.len() == 1 {
            if let GenericArgument::Type(ty) = &type_args[0] {
                if let Type::Path(type_path) = ty {
                    let current_param = &type_path.path.segments[0].ident;
                    let other_params: Vec<&Ident> = generic_params.iter()
                        .filter(|&p| p != current_param)
                        .collect();
                    
                    if !other_params.is_empty() {
                        let replacement = other_params.choose(&mut thread_rng()).unwrap();
                        new_args.clear();
                        new_args.push(GenericArgument::Type(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(replacement.clone()),
                        })));
                        Some(new_args)
                    } else {
                        None
                    }
                } else {
                    None
                }
            } else {
                None
            }
        } else {
            None
        }
    }

    fn build_tait_type(&self, ident: &Ident, args: &Punctuated<syn::GenericArgument, token::Comma>) -> Ident {
        ident.clone()
    }
}