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

pub struct Array_Slice_Type_Manipulation_23;

impl Mutator for Array_Slice_Type_Manipulation_23 {
    fn name(&self) -> &str {
        "Array_Slice_Type_Manipulation_23"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_reference) = &*pat_type.ty {
                            if let Type::Slice(type_slice) = &*type_reference.elem {
                                let array_type: Type = parse_quote!([#type_slice; 3]);
                                pat_type.ty = Box::new(array_type);
                            }
                        } else if let Type::Array(type_array) = &*pat_type.ty {
                            let slice_type: Type = parse_quote!(&[#type_array.elem]);
                            pat_type.ty = Box::new(slice_type);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function parameters with array or slice types, swapping between them. This transformation tests the compiler's handling of array and slice conversions, potentially revealing issues in type handling and optimizations during the MIR phase."
    }
}