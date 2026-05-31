use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Attribute_254;

impl Mutator for Replace_Attribute_254 {
    fn name(&self) -> &str {
        "Replace_Attribute_254"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut new_attributes = Vec::new();
                for attribute in &func.attrs {
                    if attribute.path.is_ident("proc_macro_attribute") {
                        new_attributes.push(Attribute {
                            pound_token: attribute.pound_token,
                            path: parse_quote!(proc_macro),
                            tokens: attribute.tokens.clone(),
                        });
                    } else {
                        new_attributes.push(attribute.clone());
                    }
                }
                func.attrs = new_attributes;
            }
            if let Item::Struct(struct_) = item {
                let mut new_attributes = Vec::new();
                for attribute in &struct_.attrs {
                    if attribute.path.is_ident("proc_macro_attribute") {
                        new_attributes.push(Attribute {
                            pound_token: attribute.pound_token,
                            path: parse_quote!(proc_macro),
                            tokens: attribute.tokens.clone(),
                        });
                    } else {
                        new_attributes.push(attribute.clone());
                    }
                }
                struct_.attrs = new_attributes;
            }
            if let Item::Enum(enum_) = item {
                let mut new_attributes = Vec::new();
                for attribute in &enum_.attrs {
                    if attribute.path.is_ident("proc_macro_attribute") {
                        new_attributes.push(Attribute {
                            pound_token: attribute.pound_token,
                            path: parse_quote!(proc_macro),
                            tokens: attribute.tokens.clone(),
                        });
                    } else {
                        new_attributes.push(attribute.clone());
                    }
                }
                enum_.attrs = new_attributes;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing attributes with new ones. It targets the attribute syntax in Rust code and replaces `#[proc_macro_attribute]` with `#[proc_macro]`. This transformation aims to test the compiler's handling of different attribute combinations and declarations."
    }
}