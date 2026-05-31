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

pub struct Move_Default_Generic_Param_86;

impl Mutator for Move_Default_Generic_Param_86 {
    fn name(&self) -> &str {
        "Move_Default_Generic_Param_86"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                let generics = &mut s.generics;
                let mut params: Vec<_> = generics.params.into_iter().collect();
                for i in 0..params.len() {
                    if let syn::GenericParam::Const(c) = &params[i] {
                        if c.default.is_some() && i != params.len() - 1 {
                            let param = params.remove(i);
                            params.push(param);
                            if let syn::GenericParam::Const(c) = params.last_mut().unwrap() {
                                c.ty = parse_quote!(str);
                            }
                            break;
                        }
                    }
                }
                generics.params = Punctuated::from_iter(params);
            } else if let syn::Item::Enum(e) = item {
                let generics = &mut e.generics;
                let mut params: Vec<_> = generics.params.into_iter().collect();
                for i in 0..params.len() {
                    if let syn::GenericParam::Const(c) = &params[i] {
                        if c.default.is_some() && i != params.len() - 1 {
                            let param = params.remove(i);
                            params.push(param);
                            if let syn::GenericParam::Const(c) = params.last_mut().unwrap() {
                                c.ty = parse_quote!(str);
                            }
                            break;
                        }
                    }
                }
                generics.params = Punctuated::from_iter(params);
            } else if let syn::Item::Fn(f) = item {
                let generics = &mut f.sig.generics;
                let mut params: Vec<_> = generics.params.into_iter().collect();
                for i in 0..params.len() {
                    if let syn::GenericParam::Const(c) = &params[i] {
                        if c.default.is_some() && i != params.len() - 1 {
                            let param = params.remove(i);
                            params.push(param);
                            if let syn::GenericParam::Const(c) = params.last_mut().unwrap() {
                                c.ty = parse_quote!(str);
                            }
                            break;
                        }
                    }
                }
                generics.params = Punctuated::from_iter(params);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies non-trailing const generic parameters with default values in structs, enums, or functions. It reorders these parameters to the trailing position and replaces their type with an invalid const generic type (e.g., `str`). This transformation stresses the compiler's handling of generic parameter ordering and type validation, potentially exposing bugs in const generic resolution and AST parsing logic."
    }
}