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

pub struct Replace_SizeOf_Generic_With_Unsized_377;

impl Mutator for Replace_SizeOf_Generic_With_Unsized_377 {
    fn name(&self) -> &str {
        "Replace_SizeOf_Generic_With_Unsized_377"
    }

    fn mutate(&self, file: &mut syn::File) {
        struct Visitor;
        impl<'ast> VisitMut for Visitor {
            fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
                if let Expr::Path(func_path) = call.func.as_mut() {
                    let segments = &mut func_path.path.segments;
                    if segments.len() >= 3 {
                        if segments[0].ident == "core" && 
                           segments[1].ident == "mem" && 
                           segments[2].ident == "size_of" {
                            if let PathArguments::AngleBracketed(args) = &mut segments[2].arguments {
                                if let Some(first_arg) = args.args.iter_mut().next() {
                                    if let GenericArgument::Type(ty) = first_arg {
                                        *ty = parse_quote!(dyn std::fmt::Debug);
                                    }
                                }
                            }
                        }
                    }
                    visit_mut::visit_expr_call_mut(self, call);
                }
            }
        }

        let mut visitor = Visitor;
        visitor.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces sized generic types in `core::mem::size_of::<T>()` with unsized `dyn Trait` types like `dyn Debug`. This transformation targets const expressions where size_of is used to calculate array lengths or type bounds, potentially exposing compiler bugs in handling unsized types in const contexts and triggering ICEs during type validation."
    }
}