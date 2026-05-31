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

pub struct Attribute_Modification_In_Structs_24;

impl Mutator for Attribute_Modification_In_Structs_24 {
    fn name(&self) -> &str {
        "Attribute_Modification_In_Structs_24"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.fields.is_empty() {
                    continue;
                }
                let mut has_attribute = false;
                for field in &item_struct.fields {
                    if !field.attrs.is_empty() {
                        has_attribute = true;
                        break;
                    }
                }
                if !has_attribute {
                    for field in &mut item_struct.fields {
                        field.attrs.push(parse_quote!(#[fake_attribute::unknown]));
                    }
                    continue;
                }
                for field in &mut item_struct.fields {
                    if field.attrs.is_empty() {
                        field.attrs.push(parse_quote!(#[fake_attribute::unknown]));
                    } else {
                        let mut new_attrs = Vec::new();
                        for attr in &field.attrs {
                            new_attrs.push(attr.clone());
                            new_attrs.push(parse_quote!(#[existing_attribute::malformed]));
                        }
                        field.attrs = new_attrs;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct fields with attributes. If a struct has no field attributes, it adds a fake attribute. Otherwise, it introduces malformed attributes like `#[fake_attribute::unknown]` and `#[existing_attribute::malformed]` to existing attributes. This stresses the compiler's attribute parsing and handling, potentially triggering errors or ICEs by simulating incorrect attribute syntax and semantics."
    }
}