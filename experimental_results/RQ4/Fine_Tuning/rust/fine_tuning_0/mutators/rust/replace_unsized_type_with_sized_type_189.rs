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

pub struct Replace_Unsized_Type_With_Sized_Type_189;

impl Mutator for Replace_Unsized_Type_With_Sized_Type_189 {
    fn name(&self) -> &str {
        "Replace_Unsized_Type_With_Sized_Type_189"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let mut new_ty = None;
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(segment) = trait_bound.path.segments.last() {
                                        if segment.ident == "Iterator" {
                                            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                                for arg in &args.args {
                                                    if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                                        if type_path.path.segments.last().map_or(false, |s| s.ident == "str") {
                                                            new_ty = Some(Type::ImplTrait(TypeImplTrait {
                                                                impl_token: type_impl_trait.impl_token,
                                                                bounds: {
                                                                    let mut new_bounds = Punctuated::new();
                                                                    for b in &type_impl_trait.bounds {
                                                                        if let TypeParamBound::Trait(mut tb) = b.clone() {
                                                                            if let Some(seg) = tb.path.segments.last_mut() {
                                                                                if seg.ident == "Iterator" {
                                                                                    if let PathArguments::AngleBracketed(ref mut a) = seg.arguments {
                                                                                        for a in &mut a.args {
                                                                                            if let GenericArgument::Type(Type::Path(tp)) = a {
                                                                                                if tp.path.segments.last().map_or(false, |s| s.ident == "str") {
                                                                                                    *a = GenericArgument::Type(parse_quote!(String));
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                            new_bounds.push(TypeParamBound::Trait(tb));
                                                                        } else {
                                                                            new_bounds.push(b.clone());
                                                                        }
                                                                    }
                                                                    new_bounds
                                                                },
                                                            }));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if let Some(new_ty) = new_ty {
                            *pat_type.ty = new_ty;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function parameters using `impl Trait` with unsized types like `str`. It replaces these unsized types with sized equivalents such as `String`, which can expose issues related to type layout and handling of types that require size information at compile time. This transformation tests the compiler's ability to handle conversions from unsized to sized types within trait objects."
    }
}