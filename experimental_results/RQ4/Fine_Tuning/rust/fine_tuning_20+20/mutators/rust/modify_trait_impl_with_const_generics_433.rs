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

pub struct Modify_Trait_Impl_With_Const_Generics_433;

impl Mutator for Modify_Trait_Impl_With_Const_Generics_433 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_With_Const_Generics_433"
    }
    fn mutate(&self, file: &mut syn::File) {
        fn find_function_names(file: &syn::File) -> Vec<String> {
            let mut names = Vec::new();
            for item in &file.items {
                if let syn::Item::Fn(item_fn) = item {
                    names.push(item_fn.sig.ident.to_string());
                }
            }
            names
        }

        fn create_new_function() -> syn::ItemFn {
            parse_quote! {
                fn __mutated_const_generic() -> usize {
                    0
                }
            }
        }

        fn replace_const_with_function(
            ty: &mut syn::Type,
            function_name: &str,
        ) -> bool {
            if let syn::Type::Array(type_array) = ty {
                if let syn::Expr::Path(expr_path) = &type_array.len {
                    if expr_path.path.segments.len() == 1 {
                        let ident = &expr_path.path.segments[0].ident;
                        let new_expr: syn::Expr = parse_quote! { #function_name };
                        type_array.len = new_expr;
                        return true;
                    }
                }
            }
            false
        }

        let function_names = find_function_names(file);

        let mut found_target = false;

        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.len() == 1 {
                        let trait_name = &path.segments[0].ident;
                        for ty in &item_impl.generics.params {
                            if let syn::GenericParam::Const(const_param) = ty {
                                let const_name = &const_param.ident;
                                if let syn::Type::Array(type_array) = &*item_impl.self_ty {
                                    if let syn::Expr::Path(expr_path) = &type_array.len {
                                        if expr_path.path.segments.len() == 1 {
                                            let ident = &expr_path.path.segments[0].ident;
                                            if ident == const_name {
                                                found_target = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if found_target {
                                    break;
                                }
                            }
                        }
                        if found_target {
                            break;
                        }
                    }
                }
            }
        }

        if !found_target {
            return;
        }

        let mut chosen_function_name = None;

        for name in &function_names {
            if name != "main" {
                chosen_function_name = Some(name.clone());
                break;
            }
        }

        if chosen_function_name.is_none() {
            let new_function = create_new_function();
            file.items.insert(0, syn::Item::Fn(new_function));
            chosen_function_name = Some("__mutated_const_generic".to_string());
        }

        if let Some(function_name) = chosen_function_name {
            for item in &mut file.items {
                if let syn::Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if path.segments.len() == 1 {
                            let trait_name = &path.segments[0].ident;
                            for ty in &item_impl.generics.params {
                                if let syn::GenericParam::Const(const_param) = ty {
                                    let const_name = &const_param.ident;
                                    if replace_const_with_function(&mut item_impl.self_ty, &function_name) {
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
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations that use const generics, specifically when the const generic is used as an array size. It replaces the const generic with a function call, either reusing an existing function or introducing a new one. This transformation tests the compiler's handling of const evaluation and function-based const generics, aiming to uncover issues in trait resolution and const generic substitution."
    }
}