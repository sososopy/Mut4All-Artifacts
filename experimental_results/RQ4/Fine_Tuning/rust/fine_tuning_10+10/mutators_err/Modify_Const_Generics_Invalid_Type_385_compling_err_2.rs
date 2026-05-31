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

pub struct Modify_Const_Generics_Invalid_Type_385;

impl Mutator for Modify_Const_Generics_Invalid_Type_385 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_Invalid_Type_385"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                let generics = &mut func.sig.generics;
                for param in &mut generics.params {
                    if let GenericParam::Const(ref mut const_param) = param {
                        if let Type::Path(ref mut type_path) = const_param.ty {
                            if let Some(segment) = type_path.path.segments.last_mut() {
                                if segment.ident == "i32" {
                                    segment.ident = Ident::new("String", segment.ident.span());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies functions using const generics in their signature. It specifically targets the type used in the const generic and replaces it with an incompatible type, such as replacing 'i32' with 'String'. This aims to explore scenarios where the Rust compiler's type checking of const generics might lead to internal errors, particularly with experimental features like 'adt_const_params'. The mutation is syntactically correct but semantically incorrect, designed to trigger type errors during compilation."
    }
}