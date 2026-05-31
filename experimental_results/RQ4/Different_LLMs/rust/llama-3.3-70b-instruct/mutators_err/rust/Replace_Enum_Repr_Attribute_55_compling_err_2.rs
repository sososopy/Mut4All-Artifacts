use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct Replace_Enum_Repr_Attribute_55;

impl Mutator for Replace_Enum_Repr_Attribute_55 {
    fn name(&self) -> &str {
        "Replace_Enum_Repr_Attribute_55"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut new_attributes = Vec::new();
                for attribute in &enum_item.attrs {
                    if let Ok(meta) = attribute.parse_meta() {
                        if meta.path().is_ident("repr") {
                            // Replace or remove the repr attribute
                            let repr_types = ["u8", "u16", "u32", "u64"];
                            let mut rng = thread_rng();
                            let new_repr_type = repr_types.choose(&mut rng).unwrap();
                            let new_attribute = parse_quote! {
                                #[repr(#new_repr_type)]
                            };
                            new_attributes.push(new_attribute);
                        } else {
                            new_attributes.push(attribute.clone());
                        }
                    } else {
                        new_attributes.push(attribute.clone());
                    }
                }
                // Randomly decide whether to add a new repr attribute or not
                let mut rng = thread_rng();
                if rng.gen_bool(0.5) {
                    let new_repr_type = "u8"; // Default type
                    let new_attribute = parse_quote! {
                        #[repr(#new_repr_type)]
                    };
                    new_attributes.push(new_attribute);
                }
                enum_item.attrs = new_attributes;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum declarations and replaces or removes the repr attribute. This transformation aims to trigger bugs related to layout optimization and size calculations by changing the underlying representation of enums."
    }
}

impl syn::visit_mut::VisitMut for Replace_Enum_Repr_Attribute_55 {
    fn visit_attribute_mut(&mut self, i: &mut syn::Attribute) {
        if let Ok(meta) = i.parse_meta() {
            if meta.path().is_ident("repr") {
                let repr_types = ["u8", "u16", "u32", "u64"];
                let mut rng = thread_rng();
                let new_repr_type = repr_types.choose(&mut rng).unwrap();
                let new_attribute = parse_quote! {
                    #[repr(#new_repr_type)]
                };
                *i = new_attribute;
            }
        }
    }
}