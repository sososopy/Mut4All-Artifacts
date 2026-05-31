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

pub struct Alter_Lifetimes_In_Function_Signatures_39;

impl Mutator for Alter_Lifetimes_In_Function_Signatures_39 {
    fn name(&self) -> &str {
        "Alter_Lifetimes_In_Function_Signatures_39"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(last_segment) = type_path.path.segments.last() {
                                if let PathArguments::AngleBracketed(angle_bracketed) = &last_segment.arguments {
                                    let mut has_lifetime = false;
                                    for arg in &angle_bracketed.args {
                                        if let GenericArgument::Lifetime(_) = arg {
                                            has_lifetime = true;
                                            break;
                                        }
                                    }
                                    if has_lifetime {
                                        let new_lifetime: Lifetime = parse_quote!('static);
                                        let new_args = angle_bracketed.args.iter().map(|arg| {
                                            match arg {
                                                GenericArgument::Lifetime(_) => GenericArgument::Lifetime(new_lifetime.clone()),
                                                _ => arg.clone(),
                                            }
                                        }).collect();
                                        let new_angle_bracketed = parse_quote!(<#new_args>);
                                        let new_type_path = TypePath {
                                            qself: type_path.qself.clone(),
                                            path: {
                                                let mut new_segments = type_path.path.segments.clone();
                                                if let Some(last_segment) = new_segments.last_mut() {
                                                    last_segment.arguments = PathArguments::AngleBracketed(new_angle_bracketed);
                                                }
                                                syn::Path {
                                                    leading_colon: type_path.path.leading_colon,
                                                    segments: new_segments,
                                                }
                                            },
                                        };
                                        pat_type.ty = Box::new(Type::Path(new_type_path));
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
        "This mutator targets function signatures, specifically altering lifetime annotations in parameter types. By replacing existing lifetimes with 'static, it tests the compiler's handling of lifetime resolution and borrowing checks. This can reveal issues in the interaction between lifetimes, generic parameters, and the borrow checker, especially when dealing with complex types and HRTBs."
    }
}