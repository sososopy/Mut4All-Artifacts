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

pub struct Overlap_Const_Generics_In_Impl_400;

impl Mutator for Overlap_Const_Generics_In_Impl_400 {
    fn name(&self) -> &str {
        "Overlap_Const_Generics_In_Impl_400"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut structs_with_const_generics = Vec::new();
        let mut impls_for_structs = Vec::new();

        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                let const_params: Vec<_> = item_struct.generics.params.iter()
                    .filter_map(|param| {
                        if let GenericParam::Const(const_param) = param {
                            Some((const_param.ident.clone(), const_param.ty.clone()))
                        } else {
                            None
                        }
                    })
                    .collect();
                if const_params.len() >= 2 {
                    structs_with_const_generics.push((item_struct.ident.clone(), const_params, item_struct.generics.clone()));
                }
            }
        }

        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    if let Some(path_seg) = type_path.path.segments.last() {
                        let struct_name = &path_seg.ident;
                        if let Some((_, const_params, _)) = structs_with_const_generics.iter()
                            .find(|(name, _, _)| name == struct_name) {
                            let mut const_args = Vec::new();
                            if let PathArguments::AngleBracketed(args) = &path_seg.arguments {
                                for arg in &args.args {
                                    if let GenericArgument::Const(expr) = arg {
                                        const_args.push(expr);
                                    }
                                }
                            }
                            if const_args.len() >= 1 {
                                impls_for_structs.push((item_impl, struct_name.clone(), const_args.len(), const_params.clone()));
                            }
                        }
                    }
                }
            }
        }

        if impls_for_structs.is_empty() || structs_with_const_generics.is_empty() {
            return;
        }

        let (selected_impl, struct_name, const_args_count, available_const_params) = impls_for_structs.choose(&mut rng).unwrap();
        let struct_name = struct_name.clone();
        let available_const_params = available_const_params.clone();

        let mut struct_to_modify: Option<&mut ItemStruct> = None;
        let mut impl_to_modify = None;
        let mut pos_to_replace = None;

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    if let Some(path_seg) = type_path.path.segments.last() {
                        if path_seg.ident == struct_name {
                            if let PathArguments::AngleBracketed(args) = &path_seg.arguments {
                                let mut concrete_positions = Vec::new();
                                for (i, arg) in args.args.iter().enumerate() {
                                    if let GenericArgument::Const(expr) = arg {
                                        if let Expr::Lit(_) = expr {
                                            concrete_positions.push(i);
                                        }
                                    }
                                }
                                if !concrete_positions.is_empty() {
                                    let pos = *concrete_positions.choose(&mut rng).unwrap();
                                    impl_to_modify = Some(item_impl);
                                    pos_to_replace = Some(pos);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        if let (Some(item_impl), Some(pos)) = (impl_to_modify, pos_to_replace) {
            if let Type::Path(type_path) = &mut *item_impl.self_ty {
                if let Some(path_seg) = type_path.path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut path_seg.arguments {
                        let existing_param = available_const_params.iter()
                            .find(|(ident, ty)| {
                                let arg = &args.args[pos];
                                if let GenericArgument::Const(expr) = arg {
                                    if let Expr::Lit(expr_lit) = expr {
                                        match &expr_lit.lit {
                                            Lit::Int(_) => {
                                                if let Type::Path(type_path) = ty {
                                                    type_path.path.segments.last().map(|seg| seg.ident == "usize" || seg.ident == "isize" || seg.ident == "u32" || seg.ident == "i32").unwrap_or(false)
                                                } else {
                                                    false
                                                }
                                            }
                                            Lit::Bool(_) => {
                                                if let Type::Path(type_path) = ty {
                                                    type_path.path.segments.last().map(|seg| seg.ident == "bool").unwrap_or(false)
                                                } else {
                                                    false
                                                }
                                            }
                                            Lit::Char(_) => {
                                                if let Type::Path(type_path) = ty {
                                                    type_path.path.segments.last().map(|seg| seg.ident == "char").unwrap_or(false)
                                                } else {
                                                    false
                                                }
                                            }
                                            _ => false,
                                        }
                                    } else {
                                        false
                                    }
                                } else {
                                    false
                                }
                            });

                        if let Some((param_ident, _)) = existing_param {
                            args.args[pos] = parse_quote!(#param_ident);
                        } else {
                            for item in &mut file.items {
                                if let Item::Struct(item_struct) = item {
                                    if item_struct.ident == struct_name {
                                        let new_param_name = Ident::new("P", Span::call_site());
                                        let arg = &args.args[pos];
                                        let param_type: Type = if let GenericArgument::Const(expr) = arg {
                                            if let Expr::Lit(expr_lit) = expr {
                                                match &expr_lit.lit {
                                                    Lit::Int(_) => parse_quote!(usize),
                                                    Lit::Bool(_) => parse_quote!(bool),
                                                    Lit::Char(_) => parse_quote!(char),
                                                    _ => parse_quote!(usize),
                                                }
                                            } else {
                                                parse_quote!(usize)
                                            }
                                        } else {
                                            parse_quote!(usize)
                                        };
                                        item_struct.generics.params.push(parse_quote!(const #new_param_name: #param_type));
                                        args.args[pos] = parse_quote!(#new_param_name);
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
        "The mutation operator targets structs with const generic parameters and their impl blocks. It identifies a struct with at least two const generic parameters and an impl block using concrete constant arguments. Then, it replaces one concrete constant argument with an existing const generic parameter of matching type, or introduces a new const generic parameter to the struct if none exists. This creates overlapping impl scenarios that stress the compiler's coherence checking and const generic resolution, potentially triggering internal compiler errors in the trait solver."
    }
}