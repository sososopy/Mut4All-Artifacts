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

pub struct Array_Size_Mismatch_In_Struct_224;

impl Mutator for Array_Size_Mismatch_In_Struct_224 {
    fn name(&self) -> &str {
        "Array_Size_Mismatch_In_Struct_224"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut struct_candidates = Vec::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_array_field = false;
                for field in &item_struct.fields {
                    if let Type::Array(_) = &field.ty {
                        has_array_field = true;
                        break;
                    }
                }
                if has_array_field {
                    struct_candidates.push(item_struct.ident.clone());
                }
            }
        }
        let mut impl_candidates = Vec::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.is_ident("Default") {
                        if let Type::Path(type_path) = &*item_impl.self_ty {
                            if let Some(ident) = type_path.path.get_ident() {
                                impl_candidates.push(ident.clone());
                            }
                        }
                    }
                }
            }
        }
        let mut target_struct = None;
        let mut target_impl = None;
        if !struct_candidates.is_empty() {
            target_struct = struct_candidates.choose(&mut rng).cloned();
        }
        if !impl_candidates.is_empty() {
            target_impl = impl_candidates.choose(&mut rng).cloned();
        }
        if let Some(target_struct) = target_struct {
            for item in &mut file.items {
                if let Item::Struct(item_struct) = item {
                    if item_struct.ident == target_struct {
                        for field in &mut item_struct.fields {
                            if let Type::Array(type_array) = &mut field.ty {
                                if let Expr::Lit(expr_lit) = &mut type_array.len {
                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                        let original_size =
                                            lit_int.base10_parse::<usize>().unwrap_or(0);
                                        let new_size = if original_size > 1 {
                                            original_size - 1
                                        } else {
                                            original_size + 1
                                        };
                                        let new_size_lit =
                                            syn::LitInt::new(&new_size.to_string(), lit_int.span());
                                        expr_lit.lit = syn::Lit::Int(new_size_lit);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if let Some(target_impl) = target_impl {
            for item in &mut file.items {
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if path.is_ident("Default") {
                            if let Type::Path(type_path) = &*item_impl.self_ty {
                                if let Some(ident) = type_path.path.get_ident() {
                                    if ident == &target_impl {
                                        for impl_item in &mut item_impl.items {
                                            if let ImplItem::Fn(method) = impl_item {
                                                if method.sig.ident == "default" {
                                                    for stmt in &mut method.block.stmts {
                                                        if let Stmt::Expr(expr, _) = stmt {
                                                            if let Expr::Call(expr_call) = expr {
                                                                if let Expr::Path(expr_path) =
                                                                    &*expr_call.func
                                                                {
                                                                    if expr_path
                                                                        .path
                                                                        .is_ident(&target_impl)
                                                                    {
                                                                        for arg in
                                                                            &mut expr_call.args
                                                                        {
                                                                            if let Expr::Array(
                                                                                expr_array,
                                                                            ) = arg
                                                                            {
                                                                                if let Expr::Repeat(expr_repeat) = &mut expr_array.elems[0] {
                                                                                    if let Expr::Lit(expr_lit) = &*expr_repeat.len {
                                                                                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                                                            let original_size = lit_int.base10_parse::<usize>().unwrap_or(0);
                                                                                            let new_size = if original_size > 1 {
                                                                                                original_size - 1
                                                                                            } else {
                                                                                                original_size + 1
                                                                                            };
                                                                                            let new_size_lit = syn::LitInt::new(&new_size.to_string(), lit_int.span());
                                                                                            expr_lit.lit = syn::Lit::Int(new_size_lit);
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
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with array fields and their Default trait implementations. It randomly selects a struct with an array field and a corresponding Default impl, then introduces a mismatch between the array size in the struct definition and the array size used in the default method. This inconsistency is designed to trigger compiler errors related to type checking and array bounds, potentially exposing bugs in the compiler's handling of array sizes and trait implementations."
    }
}