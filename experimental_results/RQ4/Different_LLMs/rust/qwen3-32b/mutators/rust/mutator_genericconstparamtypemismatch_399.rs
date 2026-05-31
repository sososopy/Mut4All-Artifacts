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

pub struct Mutator_GenericConstParamTypeMismatch_399;

impl Mutator for Mutator_GenericConstParamTypeMismatch_399 {
    fn name(&self) -> &str {
        "GenericConstParamTypeMismatch_399"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut structs_with_const_params = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(struct_def) = item {
                let mut const_param_types = Vec::new();
                for param in &struct_def.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        const_param_types.push(const_param.ty.clone());
                    }
                }
                if !const_param_types.is_empty() {
                    structs_with_const_params.push((struct_def.ident.clone(), const_param_types));
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                    let last_segment = type_path.path.segments.last().unwrap();
                    for (struct_name, const_param_types) in &structs_with_const_params {
                        if last_segment.ident == *struct_name {
                            let mut impl_const_params = Vec::new();
                            for param in &mut impl_item.generics.params {
                                if let syn::GenericParam::Const(const_param) = param {
                                    impl_const_params.push(const_param);
                                }
                            }
                            for (i, const_param) in impl_const_params.iter_mut().enumerate() {
                                if i < const_param_types.len() {
                                    let original_type = &const_param_types[i];
                                    let new_type = match original_type {
                                        syn::Type::Path(type_path) if type_path.path.is_ident("i64") => parse_quote!(usize),
                                        syn::Type::Path(type_path) if type_path.path.is_ident("usize") => parse_quote!(i64),
                                        syn::Type::Path(type_path) if type_path.path.is_ident("u32") => parse_quote!(i32),
                                        syn::Type::Path(type_path) if type_path.path.is_ident("i32") => parse_quote!(u32),
                                        _ => parse_quote!(usize),
                                    };
                                    const_param.ty = new_type;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with generic const parameters and their impl blocks. It modifies the const parameter types in the impl block to differ from the struct's definition, creating type mismatches in const generics. This forces the compiler to validate conflicting types (e.g., i64 vs. usize), potentially exposing ICEs during const evaluation or layout checks."
    }
}