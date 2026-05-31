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

pub struct Mutator_Insert_Async_Block_In_Const_Array_Length_388;

impl Mutator for Mutator_Insert_Async_Block_In_Const_Array_Length_388 {
    fn name(&self) -> &str {
        "Mutator_Insert_Async_Block_In_Const_Array_Length_388"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(array_type) = &mut **pat_type.ty {
                            let original_len = array_type.len.clone();
                            let new_len = parse_quote!({ async { #original_len } });
                            array_type.len = Box::new(new_len);
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::Array(array_type) = &mut **return_type {
                        let original_len = array_type.len.clone();
                        let new_len = parse_quote!({ async { #original_len } });
                        array_type.len = Box::new(new_len);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}