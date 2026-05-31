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

pub struct Introduce_Overlapping_Generic_Impls_With_Mismatched_Const_Generics_390;

impl Mutator for Introduce_Overlapping_Generic_Impls_With_Mismatched_Const_Generics_390 {
    fn name(&self) -> &str {
        "Introduce_Overlapping_Generic_Impls_With_Mismatched_Const_Generics_390"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut structs_with_const_params = Vec::new();
        let mut impls_for_structs = Vec::new();
        let mut constants_in_file = Vec::new();

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
                if !const_params.is_empty() {
                    structs_with_const_params.push((item_struct.ident.clone(), const_params, item_struct.generics.clone()));
                }
            }
            if let Item::Const(item_const) = item {
                constants_in_file.push((item_const.ident.clone(), item_const.ty.clone()));
            }
            if let Item::Impl(item_impl) = item {
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    impls_for_structs.push((type_path.path.segments.last().unwrap().ident.clone(), item_impl.clone()));
                }
            }
        }

        let mut rng = thread_rng();
        for (struct_name, const_params, struct_generics) in structs_with_const_params {
            let relevant_impls: Vec<_> = impls_for_structs.iter()
                .filter(|(impl_struct_name, _)| *impl_struct_name == struct_name)
                .map(|(_, impl_item)| impl_item.clone())
                .collect();

            if relevant_impls.is_empty() {
                continue;
            }

            let target_impl = relevant_impls.choose(&mut rng).unwrap();
            let mut new_impl = target_impl.clone();

            let const_param_count = const_params.len();
            if const_param_count == 0 {
                continue;
            }

            let param_to_change = rng.gen_range(0..const_param_count);
            let (param_name, param_type) = &const_params[param_to_change];

            let mut path_segments = match &*new_impl.self_ty {
                Type::Path(type_path) => type_path.path.segments.clone(),
                _ => continue,
            };

            let last_segment = path_segments.last_mut().unwrap();
            let args = match &mut last_segment.arguments {
                PathArguments::AngleBracketed(angle_args) => angle_args,
                _ => continue,
            };

            let mut const_args_indices = Vec::new();
            for (i, arg) in args.args.iter_mut().enumerate() {
                if let GenericArgument::Const(_) = arg {
                    const_args_indices.push(i);
                }
            }

            if const_args_indices.len() != const_param_count {
                continue;
            }

            let target_index = const_args_indices[param_to_change];
            let current_arg = &args.args[target_index];

            let mut new_generics = new_impl.generics.clone();
            let mut found_concrete = false;
            let mut found_generic = false;

            if let GenericArgument::Const(expr) = current_arg {
                if let Expr::Lit(expr_lit) = &*expr {
                    found_concrete = true;
                } else if let Expr::Path(expr_path) = &*expr {
                    if expr_path.path.segments.len() == 1 {
                        let ident = &expr_path.path.segments[0].ident;
                        if const_params.iter().any(|(name, _)| name == ident) {
                            found_generic = true;
                        }
                    }
                }
            }

            if found_concrete {
                let new_arg = GenericArgument::Const(parse_quote!(#param_name));
                args.args[target_index] = new_arg;

                let const_param_exists = new_generics.params.iter().any(|param| {
                    if let GenericParam::Const(const_param) = param {
                        const_param.ident == *param_name
                    } else {
                        false
                    }
                });

                if !const_param_exists {
                    new_generics.params.push(parse_quote!(const #param_name: #param_type));
                }
            } else if found_generic {
                let available_constants: Vec<_> = constants_in_file.iter()
                    .filter(|(_, ty)| {
                        if let Type::Path(type_path) = &**ty {
                            type_path.path.segments.last().unwrap().ident == "usize"
                        } else {
                            false
                        }
                    })
                    .map(|(ident, _)| ident.clone())
                    .collect();

                if available_constants.is_empty() {
                    let literal = match rng.gen_range(0..2) {
                        0 => parse_quote!(0),
                        _ => parse_quote!(1),
                    };
                    args.args[target_index] = GenericArgument::Const(literal);
                } else {
                    let chosen_constant = available_constants.choose(&mut rng).unwrap();
                    args.args[target_index] = GenericArgument::Const(parse_quote!(#chosen_constant));
                }
            }

            new_impl.generics = new_generics;
            last_segment.arguments = PathArguments::AngleBracketed(args.clone());

            if let Type::Path(type_path) = &mut *new_impl.self_ty {
                type_path.path.segments = path_segments;
            }

            let position = file.items.iter().position(|item| {
                if let Item::Impl(impl_item) = item {
                    impl_item.self_ty == target_impl.self_ty
                } else {
                    false
                }
            });

            if let Some(pos) = position {
                file.items.insert(pos + 1, Item::Impl(new_impl));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs with const generic parameters and their existing impl blocks. It duplicates an impl block and modifies one const generic argument to create a potential overlap: replacing a concrete constant with a generic const parameter from the struct definition, or vice versa. This creates two impls with the same number of generic parameters but differing in whether a const argument is concrete or generic, potentially triggering compiler internal errors during overlap checking, especially with generic_const_exprs feature."
    }
}