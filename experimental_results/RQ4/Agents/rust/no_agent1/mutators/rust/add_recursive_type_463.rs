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

pub struct Add_Recursive_Type_463;

impl Mutator for Add_Recursive_Type_463 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_463"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if item_struct.ident != "Main" {
                    let recursive_type: Type = parse_quote! {
                        Box<dyn Fn(Box<dyn Fn()>)>
                    };
                    item_struct.fields.iter_mut().for_each(|field| {
                        field.ty = recursive_type.clone();
                    });
                }
            }
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let recursive_type: Type = parse_quote! {
                        Box<dyn Fn(Box<dyn Fn()>)>
                    };
                    func.sig.inputs.iter_mut().for_each(|input| {
                        if let FnArg::Typed(pat_type) = input {
                            pat_type.ty = Box::new(recursive_type.clone());
                        }
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces all field types in non-main structs and parameter types in non-main functions with a recursive type `Box<dyn Fn(Box<dyn Fn()>)>`. This transformation introduces complex recursive types that challenge the compiler's type resolution and recursion handling, potentially leading to ICEs or infinite loops during type checking."
    }
}