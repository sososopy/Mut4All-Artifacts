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

pub struct Slice_Type_Mutation_23;

impl Mutator for Slice_Type_Mutation_23 {
    fn name(&self) -> &str {
        "Slice_Type_Mutation_23"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_reference) = &*pat_type.ty {
                            if let Type::Slice(_) = &*type_reference.elem {
                                pat_type.ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: parse_quote!(Box<[i32]>),
                                }));
                            }
                        } else if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(last_segment) = type_path.path.segments.last() {
                                if last_segment.ident == "Box" {
                                    if let PathArguments::AngleBracketed(angle_bracketed_args) = &last_segment.arguments {
                                        if let Some(GenericArgument::Type(Type::Slice(_))) = angle_bracketed_args.args.first() {
                                            pat_type.ty = Box::new(Type::Reference(syn::TypeReference {
                                                and_token: token::And::default(),
                                                lifetime: None,
                                                mutability: None,
                                                elem: Box::new(Type::Slice(parse_quote!([i32]))),
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
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function parameters that are either slice types or boxed slice types. It swaps slice types with boxed slice types and vice versa. This transformation tests the compiler's handling of slice and boxed slice types, potentially revealing issues in MIR optimizations and slice handling."
    }
}