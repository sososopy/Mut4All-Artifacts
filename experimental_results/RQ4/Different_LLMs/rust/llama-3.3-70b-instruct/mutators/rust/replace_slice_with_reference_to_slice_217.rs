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

pub struct Replace_Slice_With_Reference_To_Slice_217;

impl Mutator for Replace_Slice_With_Reference_To_Slice_217 {
    fn name(&self) -> &str {
        "Replace_Slice_With_Reference_To_Slice_217"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(ref_type) = pat_type.ty.as_ref() {
                            if let Type::Slice(slice_type) = ref_type.elem.as_ref() {
                                pat_type.ty = Box::new(Type::Reference(syn::TypeReference {
                                    and_token: ref_type.and_token.clone(),
                                    lifetime: ref_type.lifetime.clone(),
                                    mutability: ref_type.mutability.clone(),
                                    elem: Box::new(Type::Reference(syn::TypeReference {
                                        and_token: token::And::default(),
                                        lifetime: None,
                                        mutability: None,
                                        elem: Box::new(Type::Slice(slice_type.clone())),
                                    })),
                                }));
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(ref_type) = pat_type.ty.as_ref() {
                                    if let Type::Slice(slice_type) = ref_type.elem.as_ref() {
                                        pat_type.ty = Box::new(Type::Reference(syn::TypeReference {
                                            and_token: ref_type.and_token.clone(),
                                            lifetime: ref_type.lifetime.clone(),
                                            mutability: ref_type.mutability.clone(),
                                            elem: Box::new(Type::Reference(syn::TypeReference {
                                                and_token: token::And::default(),
                                                lifetime: None,
                                                mutability: None,
                                                elem: Box::new(Type::Slice(slice_type.clone())),
                                            })),
                                        }));
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
        "The mutation operator replaces slice parameters with references to slices in function and method signatures. This transformation tests the compiler's handling of references to slices and may lead to issues with lifetime elision, borrowing, or type inference."
    }
}