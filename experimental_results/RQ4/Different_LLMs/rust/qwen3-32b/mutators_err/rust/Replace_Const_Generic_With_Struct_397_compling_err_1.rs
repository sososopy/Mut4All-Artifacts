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

pub struct Replace_Const_Generic_With_Struct_397;

impl Mutator for Replace_Const_Generic_With_Struct_397 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Struct_397"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_names = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                struct_names.push(s.ident.clone());
            }
        }

        if struct_names.is_empty() {
            return;
        }

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for generic_param in &mut impl_item.generics.params {
                    if let syn::GenericParam::Const(const_param) = generic_param {
                        let struct_name = &struct_names[0];
                        let new_type = parse_quote! { #struct_name };
                        *const_param.ty = Box::new(new_type);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator replaces valid const generic parameters (e.g., usize) in impl blocks with user-defined struct types. It leverages existing struct declarations to create invalid const generics, testing the compiler's handling of unsupported types in const parameter positions and potentially exposing type-checking or const evaluation bugs."
    }
}