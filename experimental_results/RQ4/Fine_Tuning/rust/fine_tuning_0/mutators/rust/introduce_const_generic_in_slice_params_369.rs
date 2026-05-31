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

pub struct Introduce_Const_Generic_In_Slice_Params_369;

impl Mutator for Introduce_Const_Generic_In_Slice_Params_369 {
    fn name(&self) -> &str {
        "Introduce_Const_Generic_In_Slice_Params_369"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &*pat_type.ty {
                            if let Type::Array(type_array) = &*type_ref.elem {
                                if let Type::Array(inner_array) = &*type_array.elem {
                                    generics.params.push(parse_quote!(const N: usize));
                                    pat_type.ty = Box::new(Type::Reference(TypeReference {
                                        elem: Box::new(Type::Array(TypeArray {
                                            bracket_token: Default::default(),
                                            elem: Box::new(Type::Array(TypeArray {
                                                bracket_token: Default::default(),
                                                elem: inner_array.elem.clone(),
                                                len: inner_array.len.clone(),
                                                semi_token: Default::default(),
                                            })),
                                            len: parse_quote!(N),
                                            semi_token: Default::default(),
                                        })),
                                        ..type_ref.clone()
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function parameters that are references to arrays or slices. It introduces a const generic parameter to replace the fixed size of the array, increasing type complexity and testing the compiler's handling of const generics in function signatures."
    }
}