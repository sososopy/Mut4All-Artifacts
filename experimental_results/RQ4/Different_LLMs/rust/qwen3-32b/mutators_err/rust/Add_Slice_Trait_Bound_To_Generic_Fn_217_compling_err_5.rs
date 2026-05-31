use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypeSlice, parse_quote,
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

pub struct Add_Slice_Trait_Bound_To_Generic_Fn_217;

impl Mutator for Add_Slice_Trait_Bound_To_Generic_Fn_217 {
    fn name(&self) -> &str {
        "Add_Slice_Trait_Bound_To_Generic_Fn_217"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut has_slice_param = false;
                let mut slice_element_types = Vec::new();
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &*pat_type.ty {
                            if let Type::Slice(type_slice) = &*type_ref.elem {
                                slice_element_types.push(*type_slice.elem.clone());
                                has_slice_param = true;
                            }
                        }
                    }
                }
                if !func.sig.generics.params.is_empty() && has_slice_param {
                    let mut where_clause = func.sig.generics.where_clause.take().unwrap_or_else(|| {
                        WhereClause {
                            where_token: token::Where::default(),
                            predicates: Punctuated::new(),
                        }
                    });
                    let clone_trait = parse_quote! { Clone };
                    for elem_ty in &slice_element_types {
                        let bounded_ty = Type::Slice(TypeSlice {
                            bracket_token: token::Bracket::default(),
                            elem: Box::new(elem_ty.clone()),
                        });
                        let predicate = WherePredicate::Type(PredicateType {
                            lifetimes: None,
                            bounded_ty: Box::new(bounded_ty),
                            colon_token: token::Colon::default(),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(TypeParamBound::Trait(TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: clone_trait.clone(),
                                }));
                                bounds
                            },
                        });
                        where_clause.predicates.push(predicate);
                    }
                    func.sig.generics.where_clause = Some(where_clause);
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let mut has_slice_param = false;
                        let mut slice_element_types = Vec::new();
                        for input in &method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(type_ref) = &*pat_type.ty {
                                    if let Type::Slice(type_slice) = &*type_ref.elem {
                                        slice_element_types.push(*type_slice.elem.clone());
                                        has_slice_param = true;
                                    }
                                }
                            }
                        }
                        if !method.sig.generics.params.is_empty() && has_slice_param {
                            let mut where_clause = method.sig.generics.where_clause.take().unwrap_or_else(|| {
                                WhereClause {
                                    where_token: token::Where::default(),
                                    predicates: Punctuated::new(),
                                }
                            });
                            let clone_trait = parse_quote! { Clone };
                            for elem_ty in &slice_element_types {
                                let bounded_ty = Type::Slice(TypeSlice {
                                    bracket_token: token::Bracket::default(),
                                    elem: Box::new(elem_ty.clone()),
                                });
                                let predicate = WherePredicate::Type(PredicateType {
                                    lifetimes: None,
                                    bounded_ty: Box::new(bounded_ty),
                                    colon_token: token::Colon::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: clone_trait.clone(),
                                        }));
                                        bounds
                                    },
                                });
                                where_clause.predicates.push(predicate);
                            }
                            method.sig.generics.where_clause = Some(where_clause);
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