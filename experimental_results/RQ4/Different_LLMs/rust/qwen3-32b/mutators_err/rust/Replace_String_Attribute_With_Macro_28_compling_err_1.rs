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

pub struct Replace_String_Attribute_With_Macro_28;

impl Mutator for Replace_String_Attribute_With_Macro_28 {
    fn name(&self) -> &str {
        "Replace_String_Attribute_With_Macro_28"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Check if the macro is already present in the file
        let macro_name = "empty_macro";
        let macro_present = file.items.iter().any(|item| {
            if let Item::Macro(mac) = item {
                if mac.mac.path.is_ident("macro_rules") {
                    let body_str = mac.mac.tokens.to_string();
                    body_str.contains(macro_name)
                } else {
                    false
                }
            } else {
                false
            }
        });

        // Add the macro if not present
        if !macro_present {
            let new_macro = parse_quote! {
                macro_rules! empty_macro {}
            };
            file.items.insert(0, Item::Macro(new_macro));
        }

        // Process all attributes in the file
        for attr in &mut file.attrs {
            if let syn::Meta::NameValue(meta) = &mut *attr.meta {
                if let syn::Expr::Lit(expr_lit) = &meta.value {
                    if let syn::Lit::Str(_) = expr_lit.lit {
                        meta.value = parse_quote!(empty_macro!());
                    }
                }
            }
        }

        // Process all items in the file
        for item in &mut file.items {
            // Process the item's own attributes
            for attr in &mut item.attrs {
                if let syn::Meta::NameValue(meta) = &mut *attr.meta {
                    if let syn::Expr::Lit(expr_lit) = &meta.value {
                        if let syn::Lit::Str(_) = expr_lit.lit {
                            meta.value = parse_quote!(empty_macro!());
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces string literals in attribute values with macro invocations. If the macro is not present, it is declared as an empty macro. This transformation leverages macro expansion to bypass static value validation, potentially exposing ICEs where the compiler expects a non-empty string but receives a malformed or absent expansion."
    }
}