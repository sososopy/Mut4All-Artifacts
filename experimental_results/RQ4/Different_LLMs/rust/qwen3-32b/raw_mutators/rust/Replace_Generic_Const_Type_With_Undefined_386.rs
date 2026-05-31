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

pub struct Replace_Generic_Const_Type_With_Undefined_386;

impl Mutator for Replace_Generic_Const_Type_With_Undefined_386 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_Type_With_Undefined_386"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        const_param.ty = Box::new(parse_quote!(__NonExistentType));
                    }
                }
            } else if let syn::Item::Const(item_const) = item {
                for param in &mut item_const.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        const_param.ty = Box::new(parse_quote!(__NonExistentType));
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for param in &mut method.sig.generics.params {
                            if let syn::GenericParam::Const(const_param) = param {
                                const_param.ty = Box::new(parse_quote!(__NonExistentType));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the type of generic const parameters in function, const item, and method definitions with an undefined type `__NonExistentType`. This creates type mismatches during predicate collection, potentially triggering ICEs in the compiler's const generic resolution logic. The transformation aims to stress-test the handling of invalid const generic types and expose bugs in type inference and trait resolution for const parameters."
    }
}