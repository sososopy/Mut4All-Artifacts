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

pub struct Modify_Const_Generics_Complexity_405;

impl Mutator for Modify_Const_Generics_Complexity_405 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_Complexity_405"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some((_, generics, _)) = &item_fn.sig.generics.split_for_impl() {
                    for param in &generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Some(Type::Array(type_array)) = item_fn.sig.inputs.iter_mut().find_map(|arg| {
                                if let FnArg::Typed(PatType { ty, .. }) = arg {
                                    if let Type::Array(type_array) = &**ty {
                                        Some(type_array)
                                    } else {
                                        None
                                    }
                                } else {
                                    None
                                }
                            }) {
                                let new_type: Type = parse_quote!((usize, [(); #const_param.ident]));
                                *type_array = TypeArray {
                                    elem: Box::new(new_type),
                                    len: type_array.len.clone(),
                                    bracket_token: type_array.bracket_token,
                                };
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions using const generics and modifies the const parameter constraint from an array type to a tuple involving the array. This adds complexity to the generic constraints and tests the compiler's handling of const generics."
    }
}