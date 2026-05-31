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

pub struct Modify_Const_Expr_In_Assoc_Type_217;

impl Mutator for Modify_Const_Expr_In_Assoc_Type_217 {
    fn name(&self) -> &str {
        "Modify_Const_Expr_In_Assoc_Type_217"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, ref generics, _)) = item_impl.trait_ {
                    if generics.segments.iter().any(|segment| matches!(segment.arguments, PathArguments::AngleBracketed(ref args) if args.args.iter().any(|arg| matches!(arg, GenericArgument::Const(_))))) {
                        for item in &mut item_impl.items {
                            if let ImplItem::Type(impl_type) = item {
                                if let Type::Path(type_path) = &mut impl_type.ty {
                                    if let Some(last_segment) = type_path.path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    if let Expr::Binary(expr_binary) = expr {
                                                        if let syn::BinOp::Add(_) = expr_binary.op {
                                                            expr_binary.op = syn::BinOp::Mul(token::Star([Span::call_site()]));
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
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct implementations that use const generics and have associated types defined within an impl block. The mutation focuses on modifying the const expression in the associated type definition by changing the mathematical operation, such as replacing addition with multiplication. This subtle change tests the compiler's robustness in handling const generics and associated types, potentially revealing internal errors."
    }
}