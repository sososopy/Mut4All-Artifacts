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

pub struct Array_To_Slice_Conversion_23;

impl Mutator for Array_To_Slice_Conversion_23 {
    fn name(&self) -> &str {
        "Array_To_Slice_Conversion_23"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(type_array) = &*pat_type.ty {
                            let elem_type = &type_array.elem;
                            pat_type.ty = Box::new(Type::Reference(syn::TypeReference {
                                and_token: token::And { spans: [Span::call_site()] },
                                lifetime: None,
                                mutability: None,
                                elem: elem_type.clone(),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function parameters that are arrays and converts them to slices. This transformation tests the compiler's handling of slices, which have different memory management and type characteristics compared to fixed-size arrays. By changing arrays to slices, potential issues in slice handling and optimizations can be exposed."
    }
}