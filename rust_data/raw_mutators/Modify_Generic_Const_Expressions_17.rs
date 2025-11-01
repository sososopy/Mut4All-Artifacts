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

pub struct Modify_Generic_Const_Expressions_17;

impl Mutator for Modify_Generic_Const_Expressions_17 {
    fn name(&self) -> &str {
        "Modify_Generic_Const_Expressions_17"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(generics) = item_fn.sig.generics.params.iter().find_map(|param| {
                    if let GenericParam::Const(const_param) = param {
                        Some(const_param)
                    } else {
                        None
                    }
                }) {
                    for input in &mut item_fn.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Array(type_array) = &mut *pat_type.ty {
                                if let Expr::Path(expr_path) = &type_array.len {
                                    if expr_path.path.is_ident(&generics.ident) {
                                        type_array.len = parse_quote!(#expr_path + #expr_path);
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
        "The mutation operator targets function signatures with generic constant expressions. It identifies functions using generic constants in the signature, specifically focusing on array sizes. The mutation modifies these expressions by performing arithmetic operations, such as doubling the constant value (e.g., N + N), to test the compiler's ability to handle complex constant evaluations."
    }
}