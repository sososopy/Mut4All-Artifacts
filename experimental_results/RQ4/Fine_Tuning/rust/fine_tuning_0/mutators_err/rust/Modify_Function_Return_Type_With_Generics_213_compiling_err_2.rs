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

pub struct Modify_Function_Return_Type_With_Generics_213;

impl Mutator for Modify_Function_Return_Type_With_Generics_213 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_With_Generics_213"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(type_path) = &**ty {
                        if let Some(last_segment) = type_path.path.segments.last() {
                            if last_segment.ident == "Iter" || last_segment.ident == "IterMut" {
                                if let PathArguments::AngleBracketed(ref mut angle_args) = last_segment.arguments {
                                    angle_args.args = angle_args.args.iter().filter(|arg| {
                                        if let GenericArgument::Type(_) = arg {
                                            false
                                        } else {
                                            true
                                        }
                                    }).cloned().collect();
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                            if let Type::Path(type_path) = &**ty {
                                if let Some(last_segment) = type_path.path.segments.last() {
                                    if last_segment.ident == "Iter" || last_segment.ident == "IterMut" {
                                        if let PathArguments::AngleBracketed(ref mut angle_args) = last_segment.arguments {
                                            angle_args.args = angle_args.args.iter().filter(|arg| {
                                                if let GenericArgument::Type(_) = arg {
                                                    false
                                                } else {
                                                    true
                                                }
                                            }).cloned().collect();
                                        }
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
        "The mutation operator modifies the return type of functions and methods that return iterators with a specified `Item` type. It removes the `Item` type parameter from the iterator type, testing the compiler's handling of generic type parameters and their constraints in iterator contexts. This transformation can lead to type inference challenges and potential ICEs if the compiler's handling of associated items or metadata is flawed."
    }
}