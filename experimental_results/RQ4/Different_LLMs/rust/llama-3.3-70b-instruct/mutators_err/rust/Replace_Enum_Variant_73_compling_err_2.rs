use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Enum_Variant_73;

impl Mutator for Replace_Enum_Variant_73 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_73"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut variants = enum_item.variants.clone();
                if !variants.is_empty() {
                    let mut rng = thread_rng();
                    let selected_variant = variants.as_mut().choose(&mut rng).unwrap();
                    let new_variant = match &selected_variant.fields {
                        syn::Fields::Unit => {
                            parse_quote! { TestSome(i32) }
                        }
                        syn::Fields::Unnamed(fields) => {
                            let new_fields = fields.unnamed.clone();
                            if new_fields.len() > 0 {
                                let mut new_field_types = Vec::new();
                                for _ in 0..new_fields.len() {
                                    new_field_types.push(parse_quote! { i32 });
                                }
                                parse_quote! { TestSome(#(#new_field_types),*) }
                            } else {
                                parse_quote! { TestSome }
                            }
                        }
                        syn::Fields::Named(fields) => {
                            let mut new_fields = fields.named.clone();
                            if new_fields.len() > 0 {
                                for field in &mut new_fields {
                                    field.ty = parse_quote! { i32 };
                                }
                                let mut new_fields_iter = new_fields.into_iter();
                                let mut new_variant = parse_quote! { TestSome { };
                                while let Some(field) = new_fields_iter.next() {
                                    new_variant = parse_quote! { #new_variant #field };
                                }
                                new_variant = parse_quote! { #new_variant } };
                                new_variant
                            } else {
                                parse_quote! { TestSome }
                            }
                        }
                    };
                    variants.push(new_variant);
                    enum_item.variants = variants;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces an existing enum variant with a new one that has a similar structure but differs in the type or number of associated values. This transformation aims to test the compiler's handling of enum variants and their associated types, potentially leading to type mismatches or inference failures."
    }
}