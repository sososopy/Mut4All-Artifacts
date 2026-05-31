use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Trait_Object_With_Concrete_Type_398;

impl Mutator for Replace_Trait_Object_With_Concrete_Type_398 {
    fn name(&self) -> &str {
        "Replace_Trait_Object_With_Concrete_Type_398"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Method(func) = impl_item {
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::TraitObject(trait_object) = &*pat_type.ty {
                                    if let Some(dyn_token) = trait_object.dyn_token {
                                        let concrete_type = find_concrete_type_for_trait(
                                            &trait_object.bounds,
                                            &file,
                                        );
                                        if let Some(concrete_type) = concrete_type {
                                            pat_type.ty = Box::new(concrete_type);
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
        "The mutation operator targets trait objects in function parameters and replaces them with concrete types that implement the trait. This transformation aims to test the compiler's ability to handle the differences between sized and unsized types, like trait objects, and to ensure that it correctly computes sizes and performs other operations that depend on type properties."
    }
}

fn find_concrete_type_for_trait(bounds: &Punctuated<TypeParamBound, Plus>, file: &syn::File) -> Option<Type> {
    for item in &file.items {
        if let syn::Item::Struct(struct_item) = item {
            if let Some(impl_item) = get_impl_for_struct(struct_item, bounds, file) {
                return Some(Type::Path(TypePath {
                    qself: None,
                    path: struct_item.ident.clone().into(),
                }));
            }
        }
    }
    None
}

fn get_impl_for_struct(struct_item: &ItemStruct, bounds: &Punctuated<TypeParamBound, Plus>, file: &syn::File) -> Option<ItemImpl> {
    for item in &file.items {
        if let syn::Item::Impl(impl_item) = item {
            if let Some(trait_path) = get_trait_path_from_bounds(bounds) {
                if let Some(trait_) = &impl_item.trait_ {
                    let impl_trait_path = trait_.1.path.clone();
                    if trait_path == impl_trait_path {
                        if let Some(generics) = &impl_item.generics {
                            if let Some(where_clause) = &generics.where_clause {
                                for predicate in &where_clause.predicates {
                                    if let syn::WherePredicate::Type(predicate) = predicate {
                                        if let syn::Type::Path(type_path) = &predicate.bounded_ty {
                                            if type_path.path.is_ident(&struct_item.ident) {
                                                return Some(impl_item.clone());
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
    None
}

fn get_trait_path_from_bounds(bounds: &Punctuated<TypeParamBound, Plus>) -> Option<SynPath> {
    for bound in bounds {
        if let TypeParamBound::Trait(trait_bound) = bound {
            return Some(trait_bound.path.clone());
        }
    }
    None
}