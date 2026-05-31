use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_Struct_Field_Attribute_24;

impl Mutator for Replace_Struct_Field_Attribute_24 {
    fn name(&self) -> &str {
        "Replace_Struct_Field_Attribute_24"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                match &mut struct_item.fields {
                    syn::Fields::Named(named_fields) => {
                        for field in &mut named_fields.named {
                            if field.attrs.is_empty() {
                                field.attrs.push(parse_quote!(#[should_panic]));
                            } else {
                                field.attrs[0] = parse_quote!(#[should_panic]);
                            }
                        }
                    },
                    syn::Fields::Unnamed(unnamed_fields) => {
                        for field in &mut unnamed_fields.unnamed {
                            if field.attrs.is_empty() {
                                field.attrs.push(parse_quote!(#[should_panic]));
                            } else {
                                field.attrs[0] = parse_quote!(#[should_panic]);
                            }
                        }
                    },
                    _ => {}
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}