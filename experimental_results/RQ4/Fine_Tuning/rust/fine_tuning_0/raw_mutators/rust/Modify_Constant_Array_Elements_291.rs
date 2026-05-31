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

pub struct Modify_Constant_Array_Elements_291;

impl Mutator for Modify_Constant_Array_Elements_291 {
    fn name(&self) -> &str {
        "Modify_Constant_Array_Elements_291"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(item_const) = item {
                if let syn::Type::Array(type_array) = &*item_const.ty {
                    if let syn::Expr::Array(expr_array) = &*item_const.expr {
                        let mut rng = thread_rng();
                        let mut new_elems = expr_array.elems.clone();

                        for elem in &mut new_elems {
                            if let syn::Expr::Tuple(expr_tuple) = elem {
                                for expr in &mut expr_tuple.elems {
                                    if let syn::Expr::Lit(expr_lit) = expr {
                                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                            let new_value: usize = rng.gen_range(0..10);
                                            *expr = syn::Expr::Lit(syn::ExprLit {
                                                attrs: vec![],
                                                lit: syn::Lit::Int(syn::LitInt::new(
                                                    &new_value.to_string(),
                                                    lit_int.span(),
                                                )),
                                            });
                                        }
                                    }
                                }
                            }
                        }

                        *item_const.expr = syn::Expr::Array(syn::ExprArray {
                            attrs: expr_array.attrs.clone(),
                            bracket_token: expr_array.bracket_token,
                            elems: new_elems,
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets constant array declarations, specifically modifying the elements within tuples of constant arrays. By introducing variations in the data, it tests the compiler's handling of constant evaluation and type checking, ensuring that the modified elements remain within the array's size and type constraints."
    }
}