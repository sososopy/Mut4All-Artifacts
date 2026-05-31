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

pub struct Modify_Const_Generic_Parameters_In_Async_Functions_244;

impl Mutator for Modify_Const_Generic_Parameters_In_Async_Functions_244 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameters_In_Async_Functions_244"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    let mut remove_indices = Vec::new();
                    for (i, param) in item_fn.sig.generics.params.iter().enumerate() {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(type_path) = &const_param.ty {
                                if type_path.path.segments.last().map_or(false, |segment| {
                                    segment.ident == "AliasType"
                                }) {
                                    remove_indices.push(i);
                                }
                            }
                        }
                    }
                    for &index in remove_indices.iter().rev() {
                        item_fn.sig.generics.params = item_fn.sig.generics.params.iter().enumerate()
                            .filter_map(|(i, param)| if i != index { Some(param.clone()) } else { None })
                            .collect();
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions with const generic parameters referencing a type alias with `impl Trait`. It removes the const generic parameter from the function signature, ensuring that the function body remains valid or is adjusted accordingly. This mutation exposes potential issues in the Rust compiler's handling of const generics and type aliasing, which can lead to unexpected behavior."
    }
}