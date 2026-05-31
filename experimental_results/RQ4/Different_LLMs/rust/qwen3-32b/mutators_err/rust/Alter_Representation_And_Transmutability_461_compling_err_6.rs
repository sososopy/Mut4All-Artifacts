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

pub struct Alter_Representation_And_Transmutability_461;

impl Mutator for Alter_Representation_And_Transmutability_461 {
    fn name(&self) -> &str {
        "Alter_Representation_And_Transmutability_461"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut new_attrs = Vec::new();
                for attr in &item_struct.attrs {
                    if attr.path().is_ident("repr") {
                        let attr_str = attr.to_token_stream().to_string();
                        let new_attr_str = modify_repr_attr(&attr_str);
                        let new_attr = parse_quote!(#new_attr_str);
                        new_attrs.push(new_attr);
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }
                item_struct.attrs = new_attrs;
            } else if let syn::Item::Enum(item_enum) = item {
                let mut new_attrs = Vec::new();
                for attr in &item_enum.attrs {
                    if attr.path().is_ident("repr") {
                        let attr_str = attr.to_token_stream().to_string();
                        let new_attr_str = modify_repr_attr(&attr_str);
                        let new_attr = parse_quote!(#new_attr_str);
                        new_attrs.push(new_attr);
                    } else {
                        new_attrs.push(attr.clone());
                    }
                }
                item_enum.attrs = new_attrs;
            }
        }

        let mut visitor = TransmutabilityVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn modify_repr_attr(attr_str: &str) -> String {
    let content = attr_str.trim_start_matches("repr(").trim_end_matches(')').trim();
    if content == "C" {
        let n = thread_rng().gen_range(1..=16);
        format!("#[repr(packed({}))]", n)
    } else if content.starts_with("packed(") {
        let packed_content = content.trim_start_matches("packed(").trim_end_matches(')').trim();
        let choice = thread_rng().gen_range(0..2);
        if choice == 0 {
            "#[repr(C)]".to_string()
        } else {
            "#[repr(Rust)]".to_string()
        }
    } else {
        attr_str.to_string()
    }
}

struct TransmutabilityVisitor;

impl<'ast> VisitMut for TransmutabilityVisitor {
    fn visit_expr_path_mut(&mut self, i: &mut ExprPath) {
        if let Some(last_segment) = i.path.segments.last() {
            if last_segment.ident == "is_transmutable" {
                if let Some(segment) = i.path.segments.last_mut() {
                    if let Some(first_segment) = i.path.segments.first() {
                        if first_segment.ident == "assert" {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                if args.args.len() >= 3 {
                                    let first = args.args[0].clone();
                                    let second = args.args[1].clone();
                                    args.args[0] = second;
                                    args.args[1] = first;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_path_mut(self, i);
    }
}