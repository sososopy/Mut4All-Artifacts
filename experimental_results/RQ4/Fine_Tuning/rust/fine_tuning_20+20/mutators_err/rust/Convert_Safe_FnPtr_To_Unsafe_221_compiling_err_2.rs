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

pub struct Convert_Safe_FnPtr_To_Unsafe_221;

impl Mutator for Convert_Safe_FnPtr_To_Unsafe_221 {
    fn name(&self) -> &str {
        "Convert_Safe_FnPtr_To_Unsafe_221"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut found = false;
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr, .. }) = &local.init {
                            if let Expr::Path(expr_path) = &**expr {
                                if expr_path.path.segments.len() == 1 {
                                    if let Some((_colon_token, ty)) = &mut local.ty {
                                        if let Type::Path(type_path) = &mut **ty {
                                            if type_path.path.segments.len() == 1 {
                                                let segment = &mut type_path.path.segments[0];
                                                if let PathArguments::Parenthesized(
                                                    parenthesized,
                                                ) = &mut segment.arguments
                                                {
                                                    if !parenthesized
                                                        .inputs
                                                        .first()
                                                        .map_or(false, |ty| {
                                                            matches!(ty, Type::Infer(_))
                                                        })
                                                    {
                                                        if found {
                                                            continue;
                                                        }
                                                        found = true;
                                                        segment.ident =
                                                            Ident::new("unsafe", Span::call_site());
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
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets safe function pointer declarations in let statements within a function body. It transforms the first occurrence of a safe function pointer into an unsafe function pointer by adding the unsafe keyword to the function type. This mutation leverages the subtle differences in how the Rust compiler handles safe and unsafe function pointers, potentially exposing bugs related to type checking, borrow checking, or code generation for unsafe code."
    }
}