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

pub struct Modify_Struct_Fields_In_Const_Context_290;

impl Mutator for Modify_Struct_Fields_In_Const_Context_290 {
    fn name(&self) -> &str {
        "Modify_Struct_Fields_In_Const_Context_290"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let has_const_param_ty = item_struct
                    .attrs
                    .iter()
                    .any(|attr| attr.path().is_ident("derive") && attr.tokens.to_string().contains("ConstParamTy"));
                if !has_const_param_ty {
                    continue;
                }
                let mut field_types: Vec<String> = Vec::new();
                for field in &item_struct.fields {
                    let ty = field.ty.to_token_stream().to_string();
                    field_types.push(ty);
                }
                let mut rng = rand::thread_rng();
                let mut selected_index = rng.gen_range(0..field_types.len());
                let mut selected_type = field_types[selected_index].clone();
                let mut new_type = selected_type.clone();
                if selected_type == "i32" {
                    new_type = "u32".to_string();
                } else if selected_type == "u32" {
                    new_type = "i32".to_string();
                } else if selected_type == "i64" {
                    new_type = "u64".to_string();
                } else if selected_type == "u64" {
                    new_type = "i64".to_string();
                } else if selected_type == "i128" {
                    new_type = "u128".to_string();
                } else if selected_type == "u128" {
                    new_type = "i128".to_string();
                } else if selected_type == "isize" {
                    new_type = "usize".to_string();
                } else if selected_type == "usize" {
                    new_type = "isize".to_string();
                } else if selected_type == "f32" {
                    new_type = "f64".to_string();
                } else if selected_type == "f64" {
                    new_type = "f32".to_string();
                }
                for (i, field) in item_struct.fields.iter_mut().enumerate() {
                    if i == selected_index {
                        field.ty = syn::parse_str(&new_type).unwrap();
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions that implement the ConstParamTy trait, specifically those used as constant generic parameters. It modifies the type of a randomly selected field within such a struct to a different but compatible type. For example, it may change an i32 field to u32 or f32 to f64. This subtle type alteration aims to exploit the const parameter evaluation system by introducing potential differences in behavior without causing direct syntax errors, thereby challenging the compiler's handling of constant generics and type resolution in const contexts."
    }
}