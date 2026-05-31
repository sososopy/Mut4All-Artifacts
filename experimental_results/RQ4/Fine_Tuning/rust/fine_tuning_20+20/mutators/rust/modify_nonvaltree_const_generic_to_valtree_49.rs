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

pub struct Modify_NonValtree_Const_Generic_To_Valtree_49;

impl Mutator for Modify_NonValtree_Const_Generic_To_Valtree_49 {
    fn name(&self) -> &str {
        "Modify_NonValtree_Const_Generic_To_Valtree_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_param_name = None;
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut found = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            if type_path
                                .path
                                .segments
                                .last()
                                .map_or(false, |seg| seg.ident == "fn")
                            {
                                found = true;
                                const_param_name = Some(const_param.ident.clone());
                            }
                        }
                    }
                }
                if found {
                    let new_generics = item_struct
                        .generics
                        .params
                        .iter()
                        .map(|param| {
                            if let GenericParam::Const(const_param) = param {
                                if const_param_name.as_ref() == Some(&const_param.ident) {
                                    GenericParam::Const(parse_quote!(const N: usize = 1))
                                } else {
                                    GenericParam::Const(const_param.clone())
                                }
                            } else {
                                param.clone()
                            }
                        })
                        .collect::<Punctuated<GenericParam, Comma>>();
                    item_struct.generics.params = new_generics;
                }
            }
        }
        if let Some(const_param_name) = const_param_name {
            for item in &mut file.items {
                if let Item::Const(item_const) = item {
                    if let Expr::Call(expr_call) = &mut *item_const.expr {
                        if let Expr::Path(expr_path) = &*expr_call.func {
                            if expr_path
                                .path
                                .segments
                                .last()
                                .map_or(false, |seg| seg.ident == "X")
                            {
                                let new_args = expr_call
                                    .args
                                    .iter()
                                    .map(|arg| {
                                        if let Expr::Path(expr_path) = arg {
                                            if expr_path
                                                .path
                                                .segments
                                                .last()
                                                .map_or(false, |seg| {
                                                    seg.ident == const_param_name
                                                })
                                            {
                                                Expr::Lit(parse_quote!(1))
                                            } else {
                                                Expr::Path(expr_path.clone())
                                            }
                                        } else {
                                            arg.clone()
                                        }
                                    })
                                    .collect::<Punctuated<Expr, Comma>>();
                                expr_call.args = new_args;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generic parameters that use non-valtree-compatible types like functions or complex expressions. It replaces such const parameters with simple valtree-compatible types, specifically changing them to `usize` with a default value of `1`. Additionally, it updates any instantiations of the struct to reflect the new const parameter configuration. This transformation ensures compatibility with valtree serialization and tests the compiler's ability to adapt to changes in const generic constraints."
    }
}