use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Assoc_Type_With_Nested_Generic_Struct_6;

impl Mutator for Replace_Assoc_Type_With_Nested_Generic_Struct_6 {
    fn name(&self) -> &str {
        "Replace_Assoc_Type_With_Nested_Generic_Struct_6"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let syn::Type::Path(type_path) = *item_type.ty {
                    let path = type_path.path.clone();
                    if path.segments.len() > 1 {
                        let new_type = parse_quote! { Pooled<#path> };
                        item_type.ty = Box::new(new_type);
                    }
                }
            } else if let syn::Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let syn::Type::Path(type_path) = *field.ty {
                        let path = type_path.path.clone();
                        if path.segments.len() > 1 {
                            let new_type = parse_quote! { NdBufferView<(), #path> };
                            field.ty = Box::new(new_type);
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