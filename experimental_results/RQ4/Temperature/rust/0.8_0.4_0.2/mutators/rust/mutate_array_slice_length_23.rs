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

pub struct Mutate_Array_Slice_Length_23;

impl Mutator for Mutate_Array_Slice_Length_23 {
    fn name(&self) -> &str {
        "Mutate_Array_Slice_Length_23"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &*pat_type.ty {
                            if let Type::Slice(type_slice) = &*type_ref.elem {
                                // Change slice to array with specific length
                                pat_type.ty = Box::new(Type::Array(syn::TypeArray {
                                    bracket_token: Default::default(),
                                    elem: type_slice.elem.clone(),
                                    len: Expr::Lit(syn::ExprLit {
                                        attrs: vec![],
                                        lit: syn::Lit::Int(syn::LitInt::new("10", Span::call_site())),
                                    }),
                                    semi_token: Default::default(),
                                }));
                            }
                        } else if let Type::Array(type_array) = &*pat_type.ty {
                            // Remove explicit length from array to make it a slice
                            pat_type.ty = Box::new(Type::Reference(syn::TypeReference {
                                and_token: Default::default(),
                                lifetime: None,
                                mutability: None,
                                elem: Box::new(Type::Slice(syn::TypeSlice {
                                    bracket_token: Default::default(),
                                    elem: type_array.elem.clone(),
                                })),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}