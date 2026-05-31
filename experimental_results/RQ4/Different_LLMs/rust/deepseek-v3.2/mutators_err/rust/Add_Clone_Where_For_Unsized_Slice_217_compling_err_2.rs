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

pub struct Add_Clone_Where_For_Unsized_Slice_217;

impl Mutator for Add_Clone_Where_For_Unsized_Slice_217 {
    fn name(&self) -> &str {
        "Add_Clone_Where_For_Unsized_Slice_217"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let type_params: Vec<Ident> = func.sig.generics.params.iter()
                    .filter_map(|param| match param {
                        GenericParam::Type(type_param) => Some(type_param.ident.clone()),
                        _ => None,
                    })
                    .collect();
                let mut has_slice_usage = false;
                let mut target_param: Option<Ident> = None;
                for param in &type_params {
                    let mut visitor = SliceVisitor {
                        target_param: param.clone(),
                        found: false,
                    };
                    visitor.visit_item_fn(func);
                    if visitor.found {
                        has_slice_usage = true;
                        target_param = Some(param.clone());
                        break;
                    }
                }
                if !has_slice_usage {
                    continue;
                }
                let param_name = target_param.unwrap();
                let generics = &mut func.sig.generics;
                let where_clause = &mut generics.where_clause;
                let mut already_exists = false;
                if let Some(wc) = where_clause {
                    for predicate in &wc.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            if let Type::Slice(slice_type) = &pred_type.bounded_ty {
                                if let Type::Path(TypePath { path, .. }) = &*slice_type.elem {
                                    if path.segments.len() == 1 && path.segments[0].ident == param_name {
                                        for bound in &pred_type.bounds {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                if trait_bound.path.segments.len() == 1 && trait_bound.path.segments[0].ident == "Clone" {
                                                    already_exists = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if already_exists {
                    continue;
                }
                let new_predicate: WherePredicate = parse_quote!([#param_name]: Clone);
                if let Some(wc) = where_clause {
                    wc.predicates.push(new_predicate);
                } else {
                    generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::from_iter(vec![new_predicate]),
                    });
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let type_params: Vec<Ident> = func.sig.generics.params.iter()
                            .filter_map(|param| match param {
                                GenericParam::Type(type_param) => Some(type_param.ident.clone()),
                                _ => None,
                            })
                            .collect();
                        let mut has_slice_usage = false;
                        let mut target_param: Option<Ident> = None;
                        for param in &type_params {
                            let mut visitor = SliceVisitor {
                                target_param: param.clone(),
                                found: false,
                            };
                            visitor.visit_impl_item_fn(func);
                            if visitor.found {
                                has_slice_usage = true;
                                target_param = Some(param.clone());
                                break;
                            }
                        }
                        if !has_slice_usage {
                            continue;
                        }
                        let param_name = target_param.unwrap();
                        let generics = &mut func.sig.generics;
                        let where_clause = &mut generics.where_clause;
                        let mut already_exists = false;
                        if let Some(wc) = where_clause {
                            for predicate in &wc.predicates {
                                if let WherePredicate::Type(pred_type) = predicate {
                                    if let Type::Slice(slice_type) = &pred_type.bounded_ty {
                                        if let Type::Path(TypePath { path, .. }) = &*slice_type.elem {
                                            if path.segments.len() == 1 && path.segments[0].ident == param_name {
                                                for bound in &pred_type.bounds {
                                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                                        if trait_bound.path.segments.len() == 1 && trait_bound.path.segments[0].ident == "Clone" {
                                                            already_exists = true;
                                                        }
                                                    }
                                                }
                            }
                                        }
                                    }
                                }
                            }
                        }
                        if already_exists {
                            continue;
                        }
                        let new_predicate: WherePredicate = parse_quote!([#param_name]: Clone);
                        if let Some(wc) = where_clause {
                            wc.predicates.push(new_predicate);
                        } else {
                            generics.where_clause = Some(WhereClause {
                                where_token: Default::default(),
                                predicates: Punctuated::from_iter(vec![new_predicate]),
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a where clause requiring Clone for an unsized type parameter that appears in a slice or DST context. It targets functions and methods with generic type parameters used as element types of slices (e.g., &[T], Box<[T]>). It adds a where clause of the form `[T]: Clone` after any existing where clauses, unless such a clause already exists. This mutation aims to trigger MIR optimizations that may incorrectly assume slices are scalar values, potentially uncovering compiler bugs related to unsized types and trait bounds."
    }
}

struct SliceVisitor {
    target_param: Ident,
    found: bool,
}

impl<'ast> Visit<'ast> for SliceVisitor {
    fn visit_type(&mut self, node: &'ast Type) {
        if let Type::Slice(slice_type) = node {
            if let Type::Path(TypePath { path, .. }) = &*slice_type.elem {
                if path.segments.len() == 1 && path.segments[0].ident == self.target_param {
                    self.found = true;
                }
            }
        }
        if let Type::Reference(type_ref) = node {
            if let Type::Slice(slice_type) = &*type_ref.elem {
                if let Type::Path(TypePath { path, .. }) = &*slice_type.elem {
                    if path.segments.len() == 1 && path.segments[0].ident == self.target_param {
                        self.found = true;
                    }
                }
            }
        }
        syn::visit::visit_type(self, node);
    }
    fn visit_path(&mut self, node: &'ast SynPath) {
        if node.segments.len() == 1 && node.segments[0].ident == "Box" {
            if let PathArguments::AngleBracketed(args) = &node.segments[0].arguments {
                for arg in &args.args {
                    if let GenericArgument::Type(ty) = arg {
                        if let Type::Slice(slice_type) = ty {
                            if let Type::Path(TypePath { path, .. }) = &*slice_type.elem {
                                if path.segments.len() == 1 && path.segments[0].ident == self.target_param {
                                    self.found = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit::visit_path(self, node);
    }
}