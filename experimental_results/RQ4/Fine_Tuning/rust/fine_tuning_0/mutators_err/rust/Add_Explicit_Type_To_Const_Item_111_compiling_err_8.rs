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

pub struct Add_Explicit_Type_To_Const_Item_111;

impl Mutator for Add_Explicit_Type_To_Const_Item_111 {
    fn name(&self) -> &str {
        "Add_Explicit_Type_To_Const_Item_111"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Item(Item::Const(item_const)) = stmt {
                        if item_const.ty.is_none() {
                            if let Expr::Lit(expr_lit) = &*item_const.expr {
                                if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                    let ty = if lit_int.base10_parse::<i32>().is_ok() {
                                        parse_quote!(i32)
                                    } else {
                                        parse_quote!(u32)
                                    };
                                    item_const.ty = Some(Box::new(ty));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies const items within functions that lack explicit type annotations. It adds an appropriate type based on the const item's value, defaulting to i32 or u32 for numeric literals. This transformation ensures const items have explicit types, reducing the risk of type inference errors and improving code clarity."
    }
}