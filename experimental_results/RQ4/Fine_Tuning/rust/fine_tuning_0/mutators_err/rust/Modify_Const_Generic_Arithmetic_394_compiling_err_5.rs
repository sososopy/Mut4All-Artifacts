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

pub struct Modify_Const_Generic_Arithmetic_394;

impl Mutator for Modify_Const_Generic_Arithmetic_394 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Arithmetic_394"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut generics: Vec<&mut ConstParam> = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                generics = item_struct.generics.params.iter_mut().filter_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        Some(const_param)
                    } else {
                        None
                    }
                }).collect();
                
                if !generics.is_empty() {
                    let mut impl_found = false;
                    for item_impl in file.items.iter_mut().filter_map(|item| {
                        if let Item::Impl(item_impl) = item {
                            Some(item_impl)
                        } else {
                            None
                        }
                    }) {
                        if let Some((_, path, _)) = &item_impl.trait_ {
                            if let Some(ident) = path.segments.first().map(|seg| &seg.ident) {
                                if ident == &item_struct.ident {
                                    impl_found = true;
                                    for const_param in &mut generics {
                                        if const_param.ty != *item_impl.generics.params.iter().filter_map(|param| {
                                            if let GenericParam::Const(impl_const_param) = param {
                                                Some(impl_const_param)
                                            } else {
                                                None
                                            }
                                        }).find(|impl_const_param| impl_const_param.ident == const_param.ident).map(|impl_const_param| &impl_const_param.ty).unwrap_or(&const_param.ty) {
                                            const_param.ty = parse_quote!(usize);
                                        }
                                    }
                                    if let Some(where_clause) = &mut item_impl.generics.where_clause {
                                        for predicate in &mut where_clause.predicates {
                                            if let WherePredicate::Type(predicate_type) = predicate {
                                                if let Type::Array(type_array) = &mut predicate_type.bounded_ty {
                                                    if let Expr::Binary(expr_binary) = &mut type_array.len {
                                                        let left = &expr_binary.left;
                                                        let right = &expr_binary.right;
                                                        expr_binary.right = parse_quote!((#right * 2) as usize);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    item_impl.items.push(parse_quote! {
                                        fn new() {
                                            let _ = X + (Y * 2) as i64;
                                        }
                                    });
                                }
                            }
                        }
                    }
                    if impl_found {
                        for const_param in generics {
                            const_param.ty = parse_quote!(i64);
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