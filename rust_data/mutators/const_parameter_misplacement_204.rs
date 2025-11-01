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

pub struct Const_Parameter_Misplacement_204;

impl Mutator for Const_Parameter_Misplacement_204 {
    fn name(&self) -> &str {
        "Const_Parameter_Misplacement_204"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                let mut const_params = Vec::new();
                let mut type_params = Vec::new();

                for param in generics.params.iter() {
                    match param {
                        GenericParam::Type(_) => type_params.push(param.clone()),
                        GenericParam::Const(_) => const_params.push(param.clone()),
                        _ => {}
                    }
                }

                if const_params.len() == 1 && type_params.len() >= 1 {
                    generics.params.clear();
                    generics.params.push(type_params[0].clone());
                    generics.params.push(const_params[0].clone());
                    for param in type_params.iter().skip(1) {
                        generics.params.push(param.clone());
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator focuses on structs with const generic parameters. It identifies structs with a single const generic parameter and at least one type parameter. The mutation involves swapping the position of the first type parameter with the const parameter, thereby testing the compiler's handling of const generics in different positions."
    }
}