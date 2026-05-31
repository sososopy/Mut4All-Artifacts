use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_2,
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

pub struct Replace_Assoc_Type_In_HRTB_With_Concrete_8;

impl Mutator for Replace_Assoc_Type_In_HRTB_With_Concrete_8 {
    fn name(&self) -> &str {
        "Replace_Assoc_Type_In_HRTB_With_Concrete_8"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut concrete_impls = Vec::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    for impl_item in &item_impl.items {
                        if let ImplItem::Type(assoc_type) = impl_item {
                            concrete_impls.push((trait_path.clone(), assoc_type.ident.clone(), assoc_type.ty.clone()));
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some(where_clause) = &mut item_impl.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(pred_type) = predicate {
                            if let Some(hr_lifetimes) = &pred_type.lifetimes {
                                if !hr_lifetimes.lifetimes.is_empty() {
                                    Self::visit_type_mut(&mut pred_type.bounded_ty, |ty| {
                                        if let Type::Path(type_path) = ty {
                                            for segment in &mut type_path.path.segments {
                                                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                    for arg in &mut args.args {
                                                        if let GenericArgument::Type(Type::Path(assoc_path)) = arg {
                                                            if assoc_path.path.segments.len() == 3 {
                                                                let segs: Vec<_> = assoc_path.path.segments.iter().collect();
                                                                if segs[1].ident == "as" {
                                    let trait_ident = &segs[2].ident;
                                    let assoc_ident = &segs[0].ident;
                                    for (trait_path, concrete_assoc_ident, concrete_ty) in &concrete_impls {
                                        if trait_path.segments.last().map(|s| &s.ident) == Some(trait_ident) && assoc_ident == concrete_assoc_ident {
                                            *arg = GenericArgument::Type((*concrete_ty).clone());
                                            break;
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with HRTB where-clauses containing associated type references. It scans the seed program for concrete trait implementations that define associated types, then replaces associated type references in HRTB bounds with those concrete types. This creates type mismatches between generic associated types and concrete implementations, potentially exposing compiler bugs in monomorphization, associated item resolution, and higher-ranked trait bound handling."
    }
}

impl Replace_Assoc_Type_In_HRTB_With_Concrete_8 {
    fn visit_type_mut<F>(ty: &mut Type, f: F)
    where
        F: Fn(&mut Type) + Copy,
    {
        f(ty);
        match ty {
            Type::Array(arr) => Self::visit_type_mut(&mut arr.elem, f),
            Type::BareFn(bare_fn) => {
                if let Return2::Type(_, ret_2) = &mut bare_fn.output {
                    Self::visit_type_mut(ret_2, f);
                }
            }
            Type::Group(group) => Self::visit_type_mut(&mut group.elem, f),
            Type::ImplTrait(impl_trait) => {
                for bound in &mut impl_trait.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        Self::visit_path_mut(&mut trait_bound.path, f);
                    }
                }
            }
            Type::Infer(_) => {}
            Type::Macro(_) => {}
            Type::Never(_) => {}
            Type::Paren(paren) => Self::visit_type_mut(&mut paren.elem, f),
            Type::Path(type_path) => {
                Self::visit_path_mut(&mut type_path.path, f);
            }
            Type::Ptr(ptr) => Self::visit_type_mut(&mut ptr.elem, f),
            Type::Reference(reference) => Self::visit_type_mut(&mut reference.elem, f),
            Type::Slice(slice) => Self::visit_type_mut(&mut slice.elem, f),
            Type::TraitObject(trait_object) => {
                for bound in &mut trait_object.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        Self::visit_path_mut(&mut trait_bound.path, f);
                    }
                }
            }
            Type::Tuple(tuple) => {
                for elem in &mut tuple.elems {
                    Self::visit_type_mut(elem, f);
                }
            }
            _ => {}
        }
    }

    fn visit_path_mut<F>(path: &mut SynPath, f: F)
    where
        F: Fn(&mut Type) + Copy,
    {
        for segment in &mut path.segments {
            match &mut segment.arguments {
                PathArguments::AngleBracketed(args) => {
                    for arg in &mut args.args {
                        if let GenericArgument::Type(ty) = arg {
                            Self::visit_type_mut(ty, f);
                        }
                    }
                }
                PathArguments::Parenthesized(args) => {
                    for input in &mut args.inputs {
                        Self::visit_type_mut(input, f);
                    }
                    if let Return2::Type(_, ret_ty) = &mut args.output {
                        Self::visit_type_mut(ret_ty, f);
                    }
                }
                _ => {}
            }
        }
    }
}