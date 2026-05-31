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

pub struct Modify_Constant_Array_Elements_291;

impl Mutator for Modify_Constant_Array_Elements_291 {
    fn name(&self) -> &str {
        "Modify_Constant_Array_Elements_291"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(item_const) = item {
                if let syn::Expr::Array(array_expr) = &*item_const.expr {
                    let mut unique_elements: HashSet<String> = HashSet::new();
                    let mut new_elements = Vec::new();
                    for elem in &array_expr.elems {
                        let elem_str = elem.to_token_stream().to_string();
                        if unique_elements.contains(&elem_str) {
                            continue;
                        }
                        unique_elements.insert(elem_str.clone());
                        new_elements.push(elem.clone());
                    }
                    let mut rng = thread_rng();
                    let mut new_elems = new_elements.clone();
                    new_elems.shuffle(&mut rng);
                    let new_array_expr = syn::Expr::Array(syn::ExprArray {
                        attrs: array_expr.attrs.clone(),
                        bracket_token: array_expr.bracket_token,
                        elems: syn::punctuated::Punctuated::from_iter(new_elems),
                    });
                    item_const.expr = Box::new(new_array_expr);
                }
            }
            if let syn::Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let syn::Type::Array(type_array) = &field.ty {
                        if let syn::Type::Tuple(type_tuple) = &*type_array.elem {
                            if type_tuple.elems.len() == 2 {
                                let mut unique_elements: HashSet<String> = HashSet::new();
                                let mut new_elements = Vec::new();
                                for elem in &type_tuple.elems {
                                    let elem_str = elem.to_token_stream().to_string();
                                    if unique_elements.contains(&elem_str) {
                                        continue;
                                    }
                                    unique_elements.insert(elem_str.clone());
                                    new_elements.push(elem.clone());
                                }
                                let mut rng = thread_rng();
                                let mut new_elems = new_elements.clone();
                                new_elems.shuffle(&mut rng);
                                let new_tuple_type = syn::Type::Tuple(syn::TypeTuple {
                                    paren_token: type_tuple.paren_token,
                                    elems: syn::punctuated::Punctuated::from_iter(new_elems),
                                });
                                field.ty = syn::Type::Array(syn::TypeArray {
                                    bracket_token: type_array.bracket_token,
                                    elem: Box::new(syn::Type::Tuple(new_tuple_type)),
                                    len: type_array.len.clone(),
                                    semi_token: type_array.semi_token,
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets constant arrays and structs with constant generic parameters that involve arrays. It identifies constant arrays, particularly those with tuple elements, and modifies the elements to ensure uniqueness and shuffle their order. This transformation affects both the array's definition and its usage in structs, aiming to test the compiler's handling of constant evaluation, type consistency, and array-based generics."
    }
}