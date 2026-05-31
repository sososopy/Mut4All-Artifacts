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

pub struct Introduce_Bound_Lifetime_In_Assoc_Types_39;

impl Mutator for Introduce_Bound_Lifetime_In_Assoc_Types_39 {
    fn name(&self) -> &str {
        "Introduce_Bound_Lifetime_In_Assoc_Types_39"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(lifetime_param) = func.sig.generics.lifetimes().next() {
                    let new_lifetime: Lifetime = parse_quote!('b);
                    func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: new_lifetime.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));

                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &mut *pat_type.ty {
                                if let Some(segment) = type_path.path.segments.last_mut() {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                                if let Some(last_segment) = type_path.path.segments.last_mut() {
                                                    if let PathArguments::AngleBracketed(inner_args) = &mut last_segment.arguments {
                                                        for inner_arg in &mut inner_args.args {
                                                            if let GenericArgument::Lifetime(lifetime) = inner_arg {
                                                                *lifetime = parse_quote!('a: 'b);
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
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a new lifetime parameter `'b` to functions with existing lifetime parameters and modifies the function signature to include `'b` as a bound lifetime. This transformation targets inherent associated types with lifetimes, aiming to test the compiler's handling of lifetime relationships and bounds, potentially exposing issues in lifetime parameterization and inference."
    }
}