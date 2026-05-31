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

pub struct Modify_Array_Size_In_Struct_681;

impl Mutator for Modify_Array_Size_In_Struct_681 {
    fn name(&self) -> &str {
        "Modify_Array_Size_In_Struct_681"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let syn::Type::Array(type_array) = &mut field.ty {
                        if let syn::Expr::Block(expr_block) = &*type_array.len {
                            let constant_value = expr_block.block.stmts.iter().filter_map(|stmt| {
                                if let Stmt::Expr(Expr::Binary(expr_binary)) = stmt {
                                    if let (Expr::Lit(left), Expr::Lit(right)) = (&*expr_binary.left, &*expr_binary.right) {
                                        if let (syn::Lit::Int(left_int), syn::Lit::Int(right_int)) = (&left.lit, &right.lit) {
                                            let left_value = left_int.base10_parse::<i64>().unwrap_or(0);
                                            let right_value = right_int.base10_parse::<i64>().unwrap_or(0);
                                            return Some(left_value + right_value);
                                        }
                                    }
                                }
                                None
                            }).next();
                            
                            if let Some(value) = constant_value {
                                type_array.len = Box::new(Expr::Lit(ExprLit {
                                    attrs: Vec::new(),
                                    lit: syn::Lit::Int(syn::LitInt::new(&value.to_string(), Span::call_site())),
                                }));
                            }
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