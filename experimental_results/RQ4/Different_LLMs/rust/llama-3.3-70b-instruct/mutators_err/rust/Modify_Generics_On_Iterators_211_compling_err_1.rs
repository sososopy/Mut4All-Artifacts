use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Modify_Generics_On_Iterators_211;

impl Mutator for Modify_Generics_On_Iterators_211 {
    fn name(&self) -> &str {
        "Modify_Generics_On_Iterators_211"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == "Iter" || segment.ident == "IterMut" || segment.ident == "IntoIter" {
                                    let mut new_type_path = type_path.clone();
                                    if let PathArguments::AngleBracketed(angle_bracketed) = &mut new_type_path.path.segments.last().unwrap().arguments {
                                        let new_type = parse_quote!(usize);
                                        angle_bracketed.args.push(GenericArgument::Type(new_type));
                                    }
                                    pat_type.ty = Box::new(Type::Path(new_type_path));
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if let Some(segment) = type_path.path.segments.last() {
                                        if segment.ident == "Iter" || segment.ident == "IterMut" || segment.ident == "IntoIter" {
                                            let mut new_type_path = type_path.clone();
                                            if let PathArguments::AngleBracketed(angle_bracketed) = &mut new_type_path.path.segments.last().unwrap().arguments {
                                                let new_type = parse_quote!(usize);
                                                angle_bracketed.args.push(GenericArgument::Type(new_type));
                                            }
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
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the use of generics on iterator types and modifies the generic type parameters to introduce potential bugs. It applies to code constructs where an iterator is returned from a function or method and the iterator type has generic type parameters. The goal is to test the compiler's handling of generic iterators and ensure that it correctly handles type checking and inference in these cases."
    }
}