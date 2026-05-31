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

pub struct Modify_Const_Generics_In_Struct_350;

impl Mutator for Modify_Const_Generics_In_Struct_350 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Struct_350"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics.params.iter_mut().find_map(|param| {
                    if let syn::GenericParam::Const(const_param) = param {
                        Some(const_param)
                    } else {
                        None
                    }
                }) {
                    let new_const_param: GenericParam = parse_quote!(const M: usize = 2);
                    item_struct.generics.params.push(new_const_param);
                    
                    if let Some(syn::Expr::Lit(expr_lit)) = &mut generics.default {
                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                            let new_expr: Expr = parse_quote!(M + #lit_int);
                            *generics.default = Some(new_expr);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}