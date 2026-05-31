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

pub struct Replace_Array_Size_With_UsizeMax_Shifted_70;

impl Mutator for Replace_Array_Size_With_UsizeMax_Shifted_70 {
    fn name(&self) -> &str {
        "Replace_Array_Size_With_UsizeMax_Shifted_70"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                for field in &mut s.fields {
                    if let Type::Array(array) = &mut field.ty {
                        array.size = Box::new(parse_quote! { usize::MAX >> 2 });
                    }
                }
            } else if let syn::Item::Const(c) = item {
                if let Type::Array(array) = &mut c.ty {
                    array.size = Box::new(parse_quote! { usize::MAX >> 2 });
                }
            } else if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(array) = &mut *pat_type.ty {
                            array.size = Box::new(parse_quote! { usize::MAX >> 2 });
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::Array(array) = &mut **return_type {
                        array.size = Box::new(parse_quote! { usize::MAX >> 2 });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}