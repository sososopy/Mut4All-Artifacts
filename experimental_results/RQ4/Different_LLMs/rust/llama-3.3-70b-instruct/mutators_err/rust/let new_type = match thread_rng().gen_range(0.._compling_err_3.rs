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

pub struct Generic_Constant_Expression_Swap_352;

impl Mutator for Generic_Constant_Expression_Swap_352 {
    fn name(&self) -> &str {
        "Generic_Constant_Expression_Swap_352"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: segments,
                            },
                        }) = &*pat_type.ty {
                            if segments.len() == 1 && segments[0].ident == "T" {
                                // Add the necessary closing bracket here
                                let new_type = match thread_rng().gen_range(0..2) {
                                    0 => {
                                        parse_quote!(i32)
                                    }
                                    1 => {
                                        parse_quote!(String)
                                    }
                                    _ => unreachable!(),
                                };
                                *pat_type.ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }
}