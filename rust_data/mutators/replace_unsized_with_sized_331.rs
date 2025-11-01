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

pub struct Replace_Unsized_With_Sized_331;

impl Mutator for Replace_Unsized_With_Sized_331 {
    fn name(&self) -> &str {
        "Replace_Unsized_With_Sized_331"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct UnsizedToSizedVisitor;

        impl VisitMut for UnsizedToSizedVisitor {
            fn visit_fn_arg_mut(&mut self, arg: &mut FnArg) {
                if let FnArg::Typed(PatType { ty, .. }) = arg {
                    if let Type::ImplTrait(TypeImplTrait { bounds, .. }) = &mut **ty {
                        for bound in bounds.iter_mut() {
                            if let TypeParamBound::Trait(TraitBound { path, .. }) = bound {
                                if let Some(last_segment) = path.segments.last_mut() {
                                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                        for arg in args.args.iter_mut() {
                                            if let GenericArgument::Type(binding_ty) = arg {
                                                if let Type::Path(TypePath { path, .. }) = binding_ty {
                                                    if path.is_ident("str") {
                                                        *binding_ty = parse_quote!(&str);
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
                syn::visit_mut::visit_fn_arg_mut(self, arg);
            }
        }

        let mut visitor = UnsizedToSizedVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function parameters using `impl Trait` with unsized types and replaces the unsized type with a sized equivalent. This transformation targets function signatures, ensuring that unsized types like `str` are replaced with sized types such as `&str`. The operator modifies the AST by visiting function arguments, checking for `impl Trait` usage, and altering the type bindings where necessary."
    }
}