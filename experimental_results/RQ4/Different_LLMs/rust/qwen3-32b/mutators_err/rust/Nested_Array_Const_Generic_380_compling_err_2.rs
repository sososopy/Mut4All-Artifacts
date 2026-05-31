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

pub struct Nested_Array_Const_Generic_380;

impl Mutator for Nested_Array_Const_Generic_380 {
    fn name(&self) -> &str {
        "Nested_Array_Const_Generic_380"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut param_count = 0;
                // Process return type
                if let syn::ReturnType::Type(_, ref mut return_type) = func.sig.output {
                    if let syn::Type::Array(array) = &**return_type {
                        if let syn::Expr::Path(_) = &*array.len {
                            param_count += 1;
                            let new_name = format_ident!("M{}", param_count);
                            let new_param = parse_quote!(const #new_name: usize);
                            func.sig.generics.params.push(new_param);
                            let inner_array = syn::TypeArray {
                                elem: array.elem.clone(),
                                len: parse_quote!(#new_name),
                                bracket_token: array.bracket_token.clone(),
                                semi_token: array.semi_token.clone(),
                            };
                            let outer_array = syn::TypeArray {
                                elem: Box::new(syn::Type::Array(inner_array)),
                                len: array.len.clone(),
                                bracket_token: array.bracket_token.clone(),
                                semi_token: array.semi_token.clone(),
                            };
                            *return_type = Box::new(syn::Type::Array(outer_array));
                        }
                    }
                }
                // Process parameters
                for input in &mut func.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        let ty = &mut pat_type.ty;
                        if let syn::Type::Array(array) = &**ty {
                            if let syn::Expr::Path(_) = &*array.len {
                                param_count += 1;
                                let new_name = format_ident!("M{}", param_count);
                                let new_param = parse_quote!(const #new_name: usize);
                                func.sig.generics.params.push(new_param);
                                let inner_array = syn::TypeArray {
                                    elem: array.elem.clone(),
                                    len: parse_quote!(#new_name),
                                    bracket_token: array.bracket_token.clone(),
                                    semi_token: array.semi_token.clone(),
                                };
                                let outer_array = syn::TypeArray {
                                    elem: Box::new(syn::Type::Array(inner_array)),
                                    len: array.len.clone(),
                                    bracket_token: array.bracket_token.clone(),
                                    semi_token: array.semi_token.clone(),
                                };
                                *ty = Box::new(syn::Type::Array(outer_array));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces array types in function parameters and return types with nested arrays by introducing a new const generic parameter. This transformation forces the compiler to evaluate the inner array's const size during type checking, potentially exposing bugs in const evaluation logic or array type resolution."
    }
}