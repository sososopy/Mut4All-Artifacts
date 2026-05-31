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

pub struct Function_Signature_Mod_With_Array_23;

impl Mutator for Function_Signature_Mod_With_Array_23 {
    fn name(&self) -> &str {
        "Function_Signature_Mod_With_Array_23"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(PatType { ty, .. }) = input {
                        if let Type::Reference(type_ref) = &**ty {
                            if let Type::Slice(type_slice) = &*type_ref.elem {
                                let array_type: Type = parse_quote!([#type_slice; 3]);
                                *ty = Box::new(array_type);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function signatures with slice parameters, transforming them into fixed-size array parameters. By converting `&[T]` to `[T; 3]`, it enforces a fixed size, potentially exposing issues in how the compiler handles arrays compared to slices, especially regarding optimizations and type processing."
    }
}