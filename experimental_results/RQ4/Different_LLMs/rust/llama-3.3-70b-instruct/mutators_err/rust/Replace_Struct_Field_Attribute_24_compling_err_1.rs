use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Some(attrs) = &mut field.attrs {
                        let mut new_attrs = Vec::new();
                        for attr in attrs {
                            if attr.path.is_ident("should_panic") {
                                new_attrs.push(Attribute {
                                    pound_token: attr.pound_token,
                                    path: parse_quote!(allow),
                                    args: parse_quote!(dead_code),
                                });
                            } else if attr.path.is_ident("allow") {
                                new_attrs.push(Attribute {
                                    pound_token: attr.pound_token,
                                    path: parse_quote!(doc),
                                    args: parse_quote!(hidden),
                                });
                            } else {
                                new_attrs.push(attr.clone());
                            }
                        }
                        *attrs = new_attrs;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the attribute of a struct field with another attribute, such as replacing #[should_panic::skip] with #[allow(dead_code)] or #[doc(hidden)], to trigger potential bugs in the compiler's attribute handling code."
    }
}