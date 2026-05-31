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

pub struct Convert_Safe_FnPtr_To_Unsafe_221;

impl Mutator for Convert_Safe_FnPtr_To_Unsafe_221 {
    fn name(&self) -> &str {
        "Convert_Safe_FnPtr_To_Unsafe_221"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Let(local) = item {
                if let Some((_, expr)) = &local.init {
                    if let syn::Expr::Path(expr_path) = &**expr {
                        if let Some(qself) = &expr_path.qself {
                            if let syn::Type::Path(type_path) = &*qself.ty {
                                if type_path.path.segments.len() == 1 {
                                    let segment = &type_path.path.segments[0];
                                    if segment.ident == "fn" {
                                        if let syn::PathArguments::Parenthesized(args) = &segment.arguments {
                                            let new_ty: syn::Type = parse_quote! {
                                                unsafe fn #args
                                            };
                                            local.ty = Some(Box::new(new_ty));
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
        "This mutation operator identifies safe function pointer declarations within let statements and converts them to unsafe function pointers by adding the `unsafe` keyword. This transformation tests the compiler's handling of unsafe function pointers, potentially uncovering issues related to safety checks and optimization behaviors."
    }
}