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

pub struct Replace_Const_Eval_With_Non_Const_Eval_424;

impl Mutator for Replace_Const_Eval_With_Non_Const_Eval_424 {
    fn name(&self) -> &str {
        "Replace_Const_Eval_With_Non_Const_Eval_424"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let syn::Expr::Array(array) = &*const_item.expr {
                    if let syn::Expr::Call(call) = &*array.len {
                        if let syn::Expr::Path(path) = &*call.expr {
                            if path.path.segments.len() == 1 && path.path.segments[0].ident == "mem" {
                                let new_expr: Expr = parse_quote! { std::mem::size_of::<usize>() };
                                const_item.expr = Box::new(syn::Expr::Array(syn::ExprArray {
                                    bracket_token: array.bracket_token,
                                    elems: array.elems,
                                    len: Box::new(new_expr),
                                }));
                            }
                        }
                    }
                }
            }
            if let syn::Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let syn::Field::Named(field_named) = field {
                        if let syn::Type::Array(array) = &*field_named.ty {
                            if let syn::Expr::Call(call) = &*array.len {
                                if let syn::Expr::Path(path) = &*call.expr {
                                    if path.path.segments.len() == 1 && path.path.segments[0].ident == "mem" {
                                        let new_expr: Expr = parse_quote! { std::mem::size_of::<usize>() };
                                        field_named.ty = Box::new(syn::Type::Array(syn::TypeArray {
                                            bracket_token: array.bracket_token,
                                            elem: array.elem,
                                            len: Box::new(new_expr),
                                            semicolon_token: array.semicolon_token,
                                        }));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant expressions used in array lengths or other contexts where a constant value is required with non-constant expressions. This transformation aims to test the compiler's handling of constant evaluation and may lead to bugs related to constant evaluation, such as incorrect type inference or incorrect code generation."
    }
}