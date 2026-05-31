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

pub struct Replace_Const_Generic_Bounds_With_Underflowing_Values_112;

impl Mutator for Replace_Const_Generic_Bounds_With_Underflowing_Values_112 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Bounds_With_Underflowing_Values_112"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::ConstPolymorphic(bound) = bound {
                                    if let syn::ConstParam::Const(const_param) = &bound.param {
                                        let underflowing_value = match const_param.ty {
                                            syn::Type::Path(syn::TypePath {
                                                path: syn::Path {
                                                    segments: syn::punctuated::Punctuated(ref segments),
                                                    ..
                                                },
                                                ..
                                            }) => {
                                                let segment = segments.first().unwrap();
                                                match segment.ident.to_string().as_str() {
                                                    "u8" => quote!(0 - 1),
                                                    "u16" => quote!(0 - 1),
                                                    "u32" => quote!(0 - 1),
                                                    "u64" => quote!(0 - 1),
                                                    "usize" => quote!(0 - 1),
                                                    "i8" => quote!(std::i8::MIN - 1),
                                                    "i16" => quote!(std::i16::MIN - 1),
                                                    "i32" => quote!(std::i32::MIN - 1),
                                                    "i64" => quote!(std::i64::MIN - 1),
                                                    "isize" => quote!(std::isize::MIN - 1),
                                                    _ => quote!(0 - 1),
                                                }
                                            }
                                            _ => quote!(0 - 1),
                                        };
                                        const_param.expr = underflowing_value;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                if let Some(where_clause) = &mut impl_item.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let syn::TypeParamBound::ConstPolymorphic(bound) = bound {
                                    if let syn::ConstParam::Const(const_param) = &bound.param {
                                        let underflowing_value = match const_param.ty {
                                            syn::Type::Path(syn::TypePath {
                                                path: syn::Path {
                                                    segments: syn::punctuated::Punctuated(ref segments),
                                                    ..
                                                },
                                                ..
                                            }) => {
                                                let segment = segments.first().unwrap();
                                                match segment.ident.to_string().as_str() {
                                                    "u8" => quote!(0 - 1),
                                                    "u16" => quote!(0 - 1),
                                                    "u32" => quote!(0 - 1),
                                                    "u64" => quote!(0 - 1),
                                                    "usize" => quote!(0 - 1),
                                                    "i8" => quote!(std::i8::MIN - 1),
                                                    "i16" => quote!(std::i16::MIN - 1),
                                                    "i32" => quote!(std::i32::MIN - 1),
                                                    "i64" => quote!(std::i64::MIN - 1),
                                                    "isize" => quote!(std::isize::MIN - 1),
                                                    _ => quote!(0 - 1),
                                                }
                                            }
                                            _ => quote!(0 - 1),
                                        };
                                        const_param.expr = underflowing_value;
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
        "The mutation operator replaces const generic bounds with underflowing values. This transformation tests the compiler's ability to handle underflowing values in const generic bounds and ensures that it reports errors correctly."
    }
}