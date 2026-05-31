use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default::Default, fs, ops::Range, panic, path::Path, process::Command};
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
                                elem: Box::new(Type::Slice(syn::TypeSlice {
                                    bracket_token: token::Bracket { span: Span::call_site() },
                                    elem: elem_type.clone(),
                                })),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function parameters that are arrays and converts them into slices. By replacing `[Type; N]` with `&[Type]`, it aims to expose potential bugs in the Rust compiler related to slice handling and MIR optimizations. This transformation tests the compiler's ability to handle slice types in function signatures and may reveal issues in type inference and optimization passes."
    }
}