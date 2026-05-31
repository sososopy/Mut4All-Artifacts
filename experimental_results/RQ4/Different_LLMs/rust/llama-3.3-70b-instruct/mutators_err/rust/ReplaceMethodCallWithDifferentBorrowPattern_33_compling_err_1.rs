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

pub struct ReplaceMethodCallWithDifferentBorrowPattern_33;

impl Mutator for ReplaceMethodCallWithDifferentBorrowPattern_33 {
    fn name(&self) -> &str {
        "ReplaceMethodCallWithDifferentBorrowPattern_33"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Method(method) = impl_item {
                        if let Expr::Call(ExprCall { args, .. }) = &method.block {
                            for arg in args {
                                if let Expr::Path(ExprPath {
                                    path: SynPath { segments, .. },
                                    ..,
                                }) = arg
                                {
                                    if let Some(segment) = segments.first() {
                                        if let Some(ident) = &segment.ident {
                                            if ident == "self" {
                                                // Replace `&self` with `&mut self`
                                                if let Type::Reference(TypeReference {
                                                    and_token: _,
                                                    lifetime: None,
                                                    mutability: false,
                                                    elem: _,
                                                }) = method.sig.inputs.first().unwrap().ty
                                                {
                                                    method.sig.inputs.first_mut().unwrap().ty = parse_quote!(mut self);
                                                }
                                                // Replace `&mut self` with `self`
                                                else if let Type::Reference(TypeReference {
                                                    and_token: _,
                                                    lifetime: None,
                                                    mutability: true,
                                                    elem: _,
                                                }) = method.sig.inputs.first().unwrap().ty
                                                {
                                                    method.sig.inputs.first_mut().unwrap().ty = parse_quote!(self);
                                                }
                                                // Replace `self` with `&self`
                                                else if let Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath {
                                                        segments: segments,
                                                        ..
                                                    },
                                                }) = method.sig.inputs.first().unwrap().ty
                                                {
                                                    method.sig.inputs.first_mut().unwrap().ty = parse_quote!(&self);
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
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces method calls on objects with different borrow patterns. It tests the borrow checker's ability to handle different borrowing scenarios by replacing `&self` with `&mut self` or `self`, and vice versa."
    }
}