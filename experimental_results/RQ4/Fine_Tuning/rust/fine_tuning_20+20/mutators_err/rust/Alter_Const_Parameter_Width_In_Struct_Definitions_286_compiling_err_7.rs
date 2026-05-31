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

pub struct Alter_Const_Parameter_Width_In_Struct_Definitions_286;

impl Mutator for Alter_Const_Parameter_Width_In_Struct_Definitions_286 {
    fn name(&self) -> &str {
        "Alter_Const_Parameter_Width_In_Struct_Definitions_286"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_structs = Vec::new();
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Struct(item_struct) = item {
                if item_struct
                    .generics
                    .params
                    .iter()
                    .any(|param| matches!(param, GenericParam::Const(_)))
                {
                    target_structs.push(index);
                }
            }
        }
        for &index in &target_structs {
            if let Item::Struct(item_struct) = &mut file.items[index] {
                let mut new_generics = item_struct.generics.clone();
                let mut changed = false;
                for param in &mut new_generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Array(type_array) = const_param.ty.as_ref() {
                            if let Expr::Path(expr_path) = &type_array.len {
                                let new_len: Expr = parse_quote! { #expr_path + 1 };
                                let new_type_array = TypeArray {
                                    elem: type_array.elem.clone(),
                                    len: new_len,
                                    bracket_token: type_array.bracket_token,
                                    semi_token: type_array.semi_token,
                                };
                                const_param.ty = Type::Array(new_type_array);
                                changed = true;
                            }
                        }
                    }
                }
                if changed {
                    item_struct.generics = new_generics;
                }
            }
        }
        let mut const_replacements = Vec::new();
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Const(item_const) = item {
                if let Type::Array(type_array) = item_const.ty.as_ref() {
                    if let Expr::Lit(expr_lit) = &type_array.len {
                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                            if let Ok(original_len) = lit_int.base10_parse::<usize>() {
                                let new_len = original_len + 1;
                                let new_ty: Type = parse_quote! { [usize; #new_len] };
                                const_replacements.push((index, new_ty));
                            }
                        }
                    }
                }
            }
        }
        for (index, new_ty) in const_replacements {
            if let Item::Const(item_const) = &mut file.items[index] {
                item_const.ty = Box::new(new_ty);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs using const generics with array types and increases the array size in the const parameter while ensuring corresponding const definitions are updated to match the new size. By altering the const generic parameter dimensions, it stresses the compiler's handling of const evaluation and type consistency across struct definitions and instantiations, potentially exposing weaknesses in const generic resolution and array size validation."
    }
}