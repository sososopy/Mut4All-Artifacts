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

pub struct Modify_Function_Type_In_Type_Alias_54;

impl Mutator for Modify_Function_Type_In_Type_Alias_54 {
    fn name(&self) -> &str {
        "Modify_Function_Type_In_Type_Alias_54"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_alias_ident = None;

        for item in &mut file.items {
            if let syn::Item::Type(type_alias) = item {
                if let Type::Path(type_path) = &*type_alias.ty {
                    if let Some(segment) = type_path.path.segments.last() {
                        if segment.ident == "FnOnce" {
                            let new_type: Type = parse_quote!(FnOnce(u32) -> u16);
                            type_alias.ty = Box::new(new_type);
                            type_alias_ident = Some(type_alias.ident.clone());
                            break;
                        }
                    }
                }
            }
        }

        if let Some(type_alias_ident) = type_alias_ident {
            for item in &mut file.items {
                if let syn::Item::Fn(func) = item {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if segment.ident == type_alias_ident {
                                        pat_type.ty = Box::new(parse_quote!(u32));
                                    }
                                }
                            }
                        }
                    }
                    if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                        if let Type::Path(type_path) = &**return_type {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == type_alias_ident {
                                    *return_type = Box::new(parse_quote!(u16));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets type aliases defining function types, specifically altering the function signature within the alias. It modifies the signature by adding a parameter and changing the return type, ensuring the new parameter is compatible with existing code constructs. The mutation aims to explore edge cases in function type aliasing, potentially triggering issues in function trait bounds and type compatibility."
    }
}