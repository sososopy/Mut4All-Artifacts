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

pub struct Replace_Const_Generic_With_Fn_Ptr_406;

impl Mutator for Replace_Const_Generic_With_Fn_Ptr_406 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Fn_Ptr_406"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut functions: Vec<(Ident, String)> = vec![];
        let mut struct_original_types: HashMap<Ident, Vec<String>> = HashMap::new();

        // Collect all functions and their return types
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let return_type_str = match &func.sig.output {
                    ReturnType::Type(_, ty) => ty.to_token_stream().to_string(),
                    ReturnType::Default => "()".to_string(),
                };
                functions.push((func.sig.ident.clone(), return_type_str));
            }
        }

        // Process struct items to collect original const parameter types
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                let mut original_types = Vec::new();
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(ty) = &const_param.ty {
                            let ty_str = ty.to_token_stream().to_string();
                            original_types.push(ty_str);
                            // Replace the type with fn() -> T
                            let new_ty = parse_quote!(fn() -> #ty);
                            *const_param.ty = Some(new_ty);
                        }
                    }
                }
                if !original_types.is_empty() {
                    struct_original_types.insert(struct_item.ident.clone(), original_types);
                }
            }
        }

        // Process impl items to replace const parameter values with function calls
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                // Skip trait impls
                if impl_item.of_trait.is_some() {
                    continue;
                }
                if let Type::Path(type_path) = &*impl_item.self_ty {
                    let struct_name = &type_path.path.segments[0].ident;
                    if let Some(original_types) = struct_original_types.get(struct_name) {
                        let impl_generics = &mut impl_item.generics;
                        let mut const_param_index = 0;
                        for param in &mut impl_generics.params {
                            if let GenericParam::Const(const_param) = param {
                                let original_type_str = &original_types[const_param_index];
                                let function = functions.iter().find(|(_, ty_str)| ty_str == original_type_str);
                                if let Some((func_ident, _)) = function {
                                    let func_call = parse_quote!(#func_ident());
                                    const_param.default = Some(Box::new(func_call));
                                }
                                const_param_index += 1;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters in structs and impls with function pointer types returning the original parameter's type. It leverages existing functions in the program to construct invalid const expressions, exploiting compiler limitations on function pointers in const parameters to trigger ICEs."
    }
}