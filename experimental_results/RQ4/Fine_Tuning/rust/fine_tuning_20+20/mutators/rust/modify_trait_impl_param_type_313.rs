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

pub struct Modify_Trait_Impl_Param_Type_313;

impl Mutator for Modify_Trait_Impl_Param_Type_313 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_Param_Type_313"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_methods: HashSet<String> = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Fn(method) = item {
                        trait_methods.insert(method.sig.ident.to_string());
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_none() {
                    continue;
                }
                for item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = item {
                        if !trait_methods.contains(&method.sig.ident.to_string()) {
                            continue;
                        }
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if let Some(segment) = type_path.path.segments.last() {
                                        let ident_str = segment.ident.to_string();
                                        let new_type: Type = match ident_str.as_str() {
                                            "u8" => parse_quote!(usize),
                                            "u16" => parse_quote!(usize),
                                            "u32" => parse_quote!(usize),
                                            "u64" => parse_quote!(usize),
                                            "u128" => parse_quote!(usize),
                                            "usize" => parse_quote!(u8),
                                            "i8" => parse_quote!(isize),
                                            "i16" => parse_quote!(isize),
                                            "i32" => parse_quote!(isize),
                                            "i64" => parse_quote!(isize),
                                            "i128" => parse_quote!(isize),
                                            "isize" => parse_quote!(i8),
                                            "f32" => parse_quote!(f64),
                                            "f64" => parse_quote!(f32),
                                            "bool" => parse_quote!(usize),
                                            "char" => parse_quote!(usize),
                                            _ => continue,
                                        };
                                        pat_type.ty = Box::new(new_type);
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
        "Identify all trait implementations within the file. For each trait implementation, locate methods that correspond to those defined in the trait. For each parameter in these methods, check if the type is a common primitive type (like integers, floats, bool, or char). If so, replace the parameter type with a different but compatible type (e.g., replace `u8` with `usize`, `f32` with `f64`, etc.). This mutation targets the consistency between trait method signatures and their implementations, potentially exposing type resolution issues or internal compiler errors related to trait method dispatch."
    }
}