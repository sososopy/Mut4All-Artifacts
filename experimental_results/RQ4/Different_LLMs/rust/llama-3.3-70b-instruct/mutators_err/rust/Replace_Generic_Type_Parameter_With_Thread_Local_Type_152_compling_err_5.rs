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

pub struct Replace_Generic_Type_Parameter_With_Thread_Local_Type_152;

impl Mutator for Replace_Generic_Type_Parameter_With_Thread_Local_Type_152 {
    fn name(&self) -> &str {
        "Replace_Generic_Type_Parameter_With_Thread_Local_Type_152"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let thread_local_type = parse_quote! {
                    thread_local! { static FOO: i32 = 0; }
                };
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        type_param.default = Some(thread_local_type);
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let thread_local_type = parse_quote! {
                            thread_local! { static FOO: i32 = 0; }
                        };
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                type_param.default = Some(thread_local_type);
                            }
                        }
                    }
                }
            }
            if let syn::Item::Struct(struct_item) = item {
                let thread_local_type = parse_quote! {
                    thread_local! { static FOO: i32 = 0; }
                };
                for field in &mut struct_item.fields {
                    match field {
                        syn::Field::Named(field) => {
                            field.ty = thread_local_type.clone();
                        }
                        syn::Field::Unnamed(field) => {
                            field.ty = thread_local_type.clone();
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic type parameters with thread-local types. This transformation aims to test the compiler's handling of thread-local types in various contexts, such as function arguments, return types, and struct fields."
    }
}