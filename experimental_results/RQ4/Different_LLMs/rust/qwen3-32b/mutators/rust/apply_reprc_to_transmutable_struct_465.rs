use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashMap, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Apply_ReprC_To_Transmutable_Struct_465;

impl Mutator for Apply_ReprC_To_Transmutable_Struct_465 {
    fn name(&self) -> &str {
        "Apply_ReprC_To_Transmutable_Struct_465"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases: HashMap<String, Type> = HashMap::new();
        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                let ty = &item_type.ty;
                let mut resolved = ty.clone();
                loop {
                    if let Type::Path(type_path) = &*resolved {
                        let last_segment = type_path.path.segments.last().unwrap();
                        let ident = last_segment.ident.to_string();
                        if let Some(alias_ty) = type_aliases.get(&ident) {
                            resolved = Box::new(alias_ty.clone());
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                type_aliases.insert(item_type.ident.to_string(), *resolved);
            }
        }

        let mut enums = HashMap::new();
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                enums.insert(item_enum.ident.to_string(), item_enum.variants.len());
            }
        }

        let mut structs_to_modify = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(expr_call) = &*expr {
                            if let Expr::Path(expr_path) = &*expr_call.func {
                                if expr_path.path.segments.len() == 2 {
                                    let first = &expr_path.path.segments[0];
                                    let second = &expr_path.path.segments[1];
                                    if first.ident == "assert" && second.ident == "is_transmutable" {
                                        if let PathArguments::AngleBracketed(args) = &second.arguments {
                                            if args.args.len() == 2 {
                                                let src_ty = &args.args[0];
                                                let dst_ty = &args.args[1];
                                                let mut resolved_dst = dst_ty.clone();
                                                loop {
                                                    if let GenericArgument::Type(ref ty) = resolved_dst {
                                                        if let Type::Path(type_path) = ty {
                                                            let last_segment = type_path.path.segments.last().unwrap();
                                                            let ident = last_segment.ident.to_string();
                                                            if let Some(alias_ty) = type_aliases.get(&ident) {
                                                                resolved_dst = GenericArgument::Type(alias_ty.clone());
                                                            } else {
                                                                break;
                                                            }
                                                        } else {
                                                            break;
                                                        }
                                                    } else {
                                                        break;
                                                    }
                                                }
                                                if let GenericArgument::Type(ref ty) = resolved_dst {
                                                    if let Type::Path(type_path) = ty {
                                                        let last_segment = type_path.path.segments.last().unwrap();
                                                        let dst_name = last_segment.ident.to_string();
                                                        if let Some(variant_count) = enums.get(&dst_name) {
                                                            if *variant_count > 1 {
                                                                if let GenericArgument::Type(src_ty) = src_ty {
                                                                    if let Type::Path(src_path) = src_ty {
                                                                        let src_name = src_path.path.segments.last().unwrap().ident.to_string();
                                                                        structs_to_modify.push(src_name);
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

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if structs_to_modify.contains(&item_struct.ident.to_string()) {
                    item_struct.attrs.push(parse_quote!(#[repr(C)]));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs used as source types in `is_transmutable` checks against enums with multiple variants and adds the `#[repr(C)]` attribute to them. This stresses the compiler's transmutability validation logic by altering memory layout assumptions, increasing the likelihood of exposing bugs in discriminant computation and layout compatibility checks."
    }
}