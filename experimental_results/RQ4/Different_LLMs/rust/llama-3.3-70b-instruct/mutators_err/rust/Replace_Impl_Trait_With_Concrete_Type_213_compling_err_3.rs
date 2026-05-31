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

pub struct Replace_Impl_Trait_With_Concrete_Type_213;

impl Mutator for Replace_Impl_Trait_With_Concrete_Type_213 {
    fn name(&self) -> &str {
        "Replace_Impl_Trait_With_Concrete_Type_213"
    }

    fn mutate(&self, file: &mut syn::File) {
        let file_clone = file.clone();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            let concrete_type = find_concrete_type(&file_clone, type_impl_trait);
                            if let Some(concrete_type) = concrete_type {
                                pat_type.ty = Box::new(concrete_type);
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for arg in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                                    let concrete_type = find_concrete_type(&file_clone, type_impl_trait);
                                    if let Some(concrete_type) = concrete_type {
                                        pat_type.ty = Box::new(concrete_type);
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
        "The mutation operator replaces `impl Trait` types in function and method arguments with concrete types that implement the trait. This transformation aims to test the compiler's handling of `impl Trait` in argument positions and its interaction with type inference and trait resolution."
    }
}

fn find_concrete_type(file: &syn::File, type_impl_trait: &TypeImplTrait) -> Option<Type> {
    let trait_path = type_impl_trait.bounds.iter().next().unwrap().to_token_stream().to_string();
    for item in &file.items {
        if let syn::Item::Struct(item_struct) = item {
            if let Some(impl_trait) = find_impl_trait(item_struct, &trait_path) {
                return Some(Type::Path(TypePath {
                    qself: None,
                    path: item_struct.ident.clone().into(),
                }));
            }
        }
    }
    None
}

fn find_impl_trait(item_struct: &syn::ItemStruct, trait_path: &str) -> Option<Type> {
    for impl_item in &item_struct.impls {
        for trait_bound in &impl_item.trait_.unwrap().bounds {
            if trait_bound.path.segments.last().unwrap().ident.to_string() == trait_path {
                return Some(Type::Path(TypePath {
                    qself: None,
                    path: item_struct.ident.clone().into(),
                }));
            }
        }
    }
    None
}