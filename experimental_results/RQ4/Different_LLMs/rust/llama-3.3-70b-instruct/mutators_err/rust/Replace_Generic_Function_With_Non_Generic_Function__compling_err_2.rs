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

pub struct Replace_Generic_Function_With_Non_Generic_Function_440;

impl Mutator for Replace_Generic_Function_With_Non_Generic_Function_440 {
    fn name(&self) -> &str {
        "Replace_Generic_Function_With_Non_Generic_Function_440"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if !func.sig.generics.params.is_empty() {
                    let generic_params = &func.sig.generics.params;
                    let first_param = &func.sig.inputs.first().unwrap();
                    if let FnArg::Typed(pat_type) = first_param {
                        let ty = &*pat_type.ty;
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath { segments, .. },
                        }) = ty
                        {
                            if let Some(segment) = segments.first() {
                                if segment.ident == "T" {
                                    let new_ty = match segment.arguments {
                                        PathArguments::AngleBracketed(ref args) => {
                                            args.args.first().unwrap().clone()
                                        }
                                        _ => unreachable!(),
                                    };
                                    func.sig.generics.params.clear();
                                    func.sig.inputs.first_mut().unwrap().as_ref().unwrap().ty =
                                        Box::new(new_ty);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic functions in the given Rust code and replaces them with non-generic functions. It applies to functions that have generic parameters and are used as arguments to higher-order functions. This transformation aims to test the compiler's handling of type parameters and bindings in different contexts."
    }
}