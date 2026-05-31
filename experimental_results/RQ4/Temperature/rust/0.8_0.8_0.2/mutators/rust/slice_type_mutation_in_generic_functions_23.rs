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

pub struct Slice_Type_Mutation_In_Generic_Functions_23;

impl Mutator for Slice_Type_Mutation_In_Generic_Functions_23 {
    fn name(&self) -> &str {
        "Slice_Type_Mutation_In_Generic_Functions_23"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(PatType { ty, .. }) = input {
                        if let Type::Reference(type_reference) = &mut **ty {
                            if let Type::Slice(_) = &*type_reference.elem {
                                type_reference.elem = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path::from(Ident::new("T", Span::call_site())),
                                }));
                            }
                        } else if let Type::Path(type_path) = &mut **ty {
                            if let Some(last_segment) = type_path.path.segments.last_mut() {
                                if last_segment.ident == "Box" {
                                    if let PathArguments::AngleBracketed(angle_args) = &mut last_segment.arguments {
                                        if let Some(GenericArgument::Type(Type::Slice(_))) = angle_args.args.first() {
                                            last_segment.arguments = PathArguments::None;
                                            type_path.path.segments.push(PathSegment {
                                                ident: Ident::new("T", Span::call_site()),
                                                arguments: PathArguments::None,
                                            });
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
        "This mutation operator identifies function parameters in generic functions that use slice types `&[T]` or `Box<[T]>` and replaces them with non-slice types such as a simple reference `&T`. By doing so, it attempts to explore how altering the expected slice type affects the compiler's optimization and type generalization processes, potentially leading to different code paths in compilation and exposing edge cases in type inference."
    }
}