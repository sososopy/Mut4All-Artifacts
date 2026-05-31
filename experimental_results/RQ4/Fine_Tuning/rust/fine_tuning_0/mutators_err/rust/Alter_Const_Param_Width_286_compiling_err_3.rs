use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Alter_Const_Param_Width_286;

impl Mutator for Alter_Const_Param_Width_286 {
    fn name(&self) -> &str {
        "Alter_Const_Param_Width_286"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(generics) = item_struct.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Array(type_array) = &const_param.ty {
                            return Some((const_param, type_array));
                        }
                    }
                    None
                }) {
                    let (const_param, type_array) = generics;
                    if let Expr::Lit(expr_lit) = &type_array.len {
                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                            let new_size = lit_int.base10_parse::<usize>().unwrap_or(1) + 1;
                            let new_size_expr: Expr = parse_quote! { #new_size };
                            const_param.ty = Type::Array(TypeArray {
                                elem: type_array.elem.clone(),
                                len: new_size_expr,
                                bracket_token: type_array.bracket_token,
                                semi_token: type_array.semi_token,
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator alters the size of array types in const generic parameters of structs, potentially leading to type mismatches during instantiation. It challenges the compiler's ability to handle const generics with modified array sizes, which can expose issues in type checking and array size handling."
    }
}