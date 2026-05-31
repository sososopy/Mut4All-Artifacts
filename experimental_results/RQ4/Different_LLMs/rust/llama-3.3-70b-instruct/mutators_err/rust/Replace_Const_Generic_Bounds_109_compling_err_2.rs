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

pub struct Replace_Const_Generic_Bounds_109;

impl Mutator for Replace_Const_Generic_Bounds_109 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Bounds_109"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(param) = param {
                        if let Some(ty) = &param.ty {
                            let mut new_ty = ty.clone();
                            if let Type::Path(type_path) = &mut new_ty {
                                for segment in &mut type_path.path.segments {
                                    if segment.ident == "Copy" {
                                        segment.ident = Ident::new("Clone", segment.ident.span());
                                    }
                                }
                            }
                            param.ty = Some(new_ty);
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(param) = param {
                                if let Some(ty) = &param.ty {
                                    let mut new_ty = ty.clone();
                                    if let Type::Path(type_path) = &mut new_ty {
                                        for segment in &mut type_path.path.segments {
                                            if segment.ident == "Copy" {
                                                segment.ident = Ident::new("Clone", segment.ident.span());
                                            }
                                        }
                                    }
                                    param.ty = Some(new_ty);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing bounds on const generic parameters with alternative bounds, potentially altering the constraints on the generic parameters. It aims to test the compiler's handling of const generics and their bounds, and may lead to ICEs or inference failures in the trait resolution system."
    }
}