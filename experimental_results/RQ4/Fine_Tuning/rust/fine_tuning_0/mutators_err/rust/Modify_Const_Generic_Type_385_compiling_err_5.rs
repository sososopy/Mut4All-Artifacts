use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, BoundLifetimes, Expr, ExprCall, ExprPath,
    File, FnArg, GenericArgument, GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime,
    LifetimeParam, Local, Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt,
    TraitBound, TraitBoundModifier, Type, TypeImplTrait, TypeParamBound, TypePath,
    punctuated::Punctuated, token::Comma, token::{Paren, Plus}, visit::Visit,
};

use crate::mutator::Mutator;

pub struct Modify_Const_Generic_Type_385;

impl Mutator for Modify_Const_Generic_Type_385 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Type_385"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_ident = None;
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                struct_ident = Some(item_struct.ident.clone());
                if let Some(generics) = item_struct.generics.params.iter_mut().find_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        match &const_param.ty {
                            Type::Path(type_path) => {
                                let segment = type_path.path.segments.last().unwrap();
                                if segment.ident != "usize" && segment.ident != "bool" && segment.ident != "char" {
                                    Some(const_param)
                                } else {
                                    None
                                }
                            }
                            _ => None,
                        }
                    } else {
                        None
                    }
                }) {
                    generics.ty = parse_quote!(usize);
                }
            }
        }

        if let Some(struct_ident) = struct_ident {
            if let Some(instantiation) = file.items.iter_mut().find_map(|item| {
                if let Item::Const(item_const) = item {
                    if let Expr::Struct(expr_struct) = &mut *item_const.expr {
                        if expr_struct.path == struct_ident.clone().into() {
                            Some(expr_struct)
                        } else {
                            None
                        }
                    } else {
                        None
                    }
                } else {
                    None
                }
            }) {
                instantiation.fields.clear();
                instantiation.fields.push(parse_quote!(1));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the type of const generic parameters in struct definitions to `usize`, and updates any instantiations of the struct accordingly. This transformation tests the compiler's handling of const generics and type constraints, potentially exposing issues in const evaluation and type inference."
    }
}