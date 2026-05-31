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
    BinOp,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Array_Length_Expression_190;

impl Mutator for Modify_Array_Length_Expression_190 {
    fn name(&self) -> &str {
        "Modify_Array_Length_Expression_190"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let Some(expr) = get_array_length_expression(field) {
                        let modified_expr = modify_expression(expr);
                        set_array_length_expression(field, modified_expr);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array declarations with length expressions that involve constants or trait bounds. It modifies the length expression to introduce a potential issue with const evaluation or layout computation, helping to expose bugs related to const evaluation or layout computation."
    }
}

fn get_array_length_expression(field: &mut syn::Field) -> Option<Expr> {
    if let Some(expr) = get_expr_from_field(field) {
        if let Expr::Array(expr_array) = expr {
            let len = expr_array.elems.len();
            return Some(parse_quote!(#len));
        }
    }
    None
}

fn modify_expression(expr: Expr) -> Expr {
    let mut rng = thread_rng();
    match rng.gen_range(0..2) {
        0 => {
            // Add 1 to the expression
            let plus_token = token::Plus::default();
            let one_literal = parse_quote!(1);
            Expr::Binary(ExprBinary {
                attrs: vec![],
                op: BinOp::Add(plus_token),
                left: Box::new(expr),
                right: Box::new(one_literal),
            })
        }
        1 => {
            // Wrap the expression in a block
            let block = parse_quote!({
                let x: Option<i32> = None;
                #expr
            });
            block
        }
        _ => unreachable!(),
    }
}

fn set_array_length_expression(field: &mut syn::Field, expr: Expr) {
    if let Some(expr_field) = get_expr_from_field(field) {
        if let Expr::Array(expr_array) = expr_field {
            let len = expr;
            let new_array = parse_quote!([u8; #len]);
            field.ty = new_array;
        }
    }
}

fn get_expr_from_field(field: &mut syn::Field) -> Option<Expr> {
    match field.ty {
        Type::Array(ref mut type_array) => Some(parse_quote!(#type_array.len)),
        Type::Path(type_path) => {
            if let Some(segment) = type_path.path.segments.last() {
                if segment.ident == "Array" {
                    if let PathArguments::AngleBracketed(angle_bracketed) = &segment.arguments {
                        if let Some(arg) = angle_bracketed.args.first() {
                            if let GenericArgument::Type(Type::Array(type_array)) = arg {
                                return Some(parse_quote!(#type_array.len));
                            }
                        }
                    }
                }
            }
            None
        }
        _ => None,
    }
}