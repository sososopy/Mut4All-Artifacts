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

pub struct Modify_Const_Generics_In_Array_Types_35;

impl Mutator for Modify_Const_Generics_In_Array_Types_35 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Array_Types_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(fields) = &mut item_struct.fields.iter_mut().next() {
                    if let Type::Array(type_array) = &mut fields.ty {
                        if let Expr::Path(expr_path) = &*type_array.len {
                            if let Some(segment) = expr_path.path.segments.last() {
                                let ident = &segment.ident;
                                let new_expr: Expr = parse_quote! { #ident + 1 };
                                type_array.len = Box::new(new_expr);
                            }
                        }
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(type_array) = &mut *pat_type.ty {
                            if let Expr::Path(expr_path) = &*type_array.len {
                                if let Some(segment) = expr_path.path.segments.last() {
                                    let ident = &segment.ident;
                                    let new_expr: Expr = parse_quote! { #ident - 1 };
                                    type_array.len = Box::new(new_expr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets array types in struct definitions and function signatures that use const generics. It modifies the const generic parameter by either increasing or decreasing its value, testing the compiler's handling of const generic arithmetic and potential interactions with other const parameters."
    }
}