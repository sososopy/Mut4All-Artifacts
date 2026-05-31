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

pub struct Alter_Array_Size_In_Generic_Constants_368;

impl Mutator for Alter_Array_Size_In_Generic_Constants_368 {
    fn name(&self) -> &str {
        "Alter_Array_Size_In_Generic_Constants_368"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                            if let syn::Type::Array(type_array) = &mut **ty {
                                if let syn::Expr::Path(ref mut expr_path) = &mut type_array.len {
                                    if let Some(segment) = expr_path.path.segments.last_mut() {
                                        if segment.ident == "N" {
                                            segment.ident = Ident::new("N * 2", segment.ident.span());
                                        }
                                    }
                                }
                            }
                        }
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let syn::Type::Array(type_array) = &mut *pat_type.ty {
                                    if let syn::Expr::Path(ref mut expr_path) = &mut type_array.len {
                                        if let Some(segment) = expr_path.path.segments.last_mut() {
                                            if segment.ident == "N" {
                                                segment.ident = Ident::new("N * 2", segment.ident.span());
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets arrays with generic constants in function signatures or bodies, specifically doubling the size of arrays by modifying the size expression from `N` to `N * 2`. This aims to stress-test the compiler's handling of array sizes in generics, potentially uncovering bugs related to array bounds, memory allocation, or type inference when array sizes are altered."
    }
}